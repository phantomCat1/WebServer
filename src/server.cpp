#include "../include/server.hpp"
#include"../include/http_request.hpp"
#include"../include/router.hpp"
#include<sys/socket.h>
#include<netdb.h>
#include<sys/types.h>
#include<unistd.h>
#include<iostream>
#include<thread>
#include<netinet/in.h>
#include<sstream>
#include<mutex>
#include<fcntl.h>
#include<errno.h>
#include<condition_variable>



void HTTP_Server::createListeningThread(){
    this->_server_socket = socket(AF_INET, SOCK_STREAM, 0);        // Create a socket using TCP (by SOCK_STREAM) and IPv4 (by AF_INET)
    if(_server_socket == -1){                                
        std::cerr<<"Error creating socket\n";               // Handle error case if socket creation fails
        return;
    }

    int flags = fcntl(_server_socket, F_GETFL, 0);          // get the flags of the socket
    if (flags == -1) {
        throw std::runtime_error("fcntl F_GETFL failed");                            // handle errors if flag retrieval failed
        close(_server_socket);
        return;
    }
    if (fcntl(_server_socket, F_SETFL, flags | O_NONBLOCK) == -1) { // set the socket fd to NONBLOCKing mode
        throw std::runtime_error("fcntl F_SETFL failed");
        close(_server_socket);
        return;
    }

    sockaddr_in server_addr{};                              // Create struct which will store information about the internet address
    server_addr.sin_family = AF_INET;                       // Fill in values about the address: first is that it uses IPv4
    server_addr.sin_port = htons(_port);                     // Next we give the port, which converts the integer port to network byte order
    server_addr.sin_addr.s_addr = INADDR_ANY;               // Basically give it any available IP adress on the system
    
    if(bind(_server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){   
        std::cerr<<"Error binding\n";                       // Try to bind the socket and output error message if failed
        return;
    }
    if (listen(_server_socket, 10) < 0) {                    // Listen for connection requests, while having a max queue of 10 pending requests
        std::ostringstream msg;
        msg << "Failed to listen on port " << _port;         // until the kernel starts dropping them 
        throw std::runtime_error(msg.str());
    }                             
                                                            
    std::cout<<"Server listening on port: "<<_port<<"\n";

    while(!_ended.load(std::memory_order_acquire)){                 // repeat while server is not stopped
        int clientfd = accept(_server_socket, nullptr,nullptr);     // accept pending client connection and create new socket file descriptor
        if(clientfd >=0) {

            std::unique_lock<std::mutex> lock(_request_mutex);       // lock the next operations and ensure mutual exclusion
            if(_pending_connections.size() <= MAX_PENDING_CONN){    // only allow a MAX_PENDING_CONN number of pending connections in the queue
                _pending_connections.push_back(clientfd);           
                cv.notify_one();                                    // Once new connection is available, push it to the queue
            } else {
                close(clientfd);                                    // if max limit of pending connections is reached, close it.
            }
            lock.unlock();
        } else {
            if(errno==EAGAIN || errno==EWOULDBLOCK){
                continue;
            } else {
                close(_server_socket);
                throw std::runtime_error("failed to accept connection");
            }
        }
    }
    close(_server_socket);
    std::cout<<"Stopped listener\n";
    
}

void HTTP_Server::handleClient(){
    int sockfd=0;
    while(!_ended.load(std::memory_order_acquire)){
        std::unique_lock<std::mutex> lk(_request_mutex);            // lock for mutual exclusion
        cv.wait(lk, [this](){                                       // use condition variable to wait for the queue to be non empty first
            return !_pending_connections.empty() || _ended.load(std::memory_order_acquire); 
        });

        if(_ended.load(std::memory_order_acquire)){
            break;
        }

        if(!_pending_connections.empty()){                          // check again for non empty queue and 
            sockfd = _pending_connections.front();                  // pop the connection that waits the longest in the queue
            _pending_connections.pop_front();
        }

        lk.unlock();
        
        Router router;
        HTTP_Request request;
        HTTP_Response response;
        char buffer[4096]={0};
        int bytes_received = recv(sockfd, buffer, sizeof(buffer), 0);   // read data from client

        if(bytes_received > 0){                                         
            std::string request_str(buffer, bytes_received);            // convert buffer to string
            request.parse(request_str);                                 // parse the request and complete the http_request object

            if(request.method.compare("GET") != 0){
                response.response_code = router.getResponseCode(501);
                response.body = "Not Implemented";
            } else if (request.path.find("/..") != std::string::npos){
                response.response_code = router.getResponseCode(403);
                response.body="Forbidden";
            } else {
                size_t pos = request.path.find_last_of('.');            //get the file extension which will represent the type of reqeste data
                if(pos != std::string::npos && pos < request.path.length()-1){
                    std::string type = request.path.substr(pos+1);
                    response = router.getResource(request.path, type);
                } else {
                    response.response_code = router.getResponseCode(400);
                    response.body = "Bad request";
                }
            }
            
            std::string response_str = response.to_string();
            send(sockfd, response_str.c_str(), response_str.size(),0);

        }
        close(sockfd);
    }
}


void HTTP_Server::start(){
    //create listening htread
    std::thread listeningThread = std::thread([this]() {this->createListeningThread(); });
    _workers.push_back(std::move(listeningThread));
    // create all worker processes
    for(int i=0; i<MAX_WORKERS; i++){
        std::thread worker = std::thread([this]() {this->handleClient(); });
        _workers.push_back(std::move(worker));
    }
    //after this just exit and return so that you can listen to cmd inputs
    std::cout<<"Successfully started server\n"<<std::endl;
}

void HTTP_Server::stop(){
    _ended.store(true, std::memory_order_release);
    cv.notify_all();
    for(auto& w: _workers){
        if(w.joinable()){
            w.join();
            std::cout<<"Stopped worker\n";
        } else {
            std::ostringstream msg;
            msg << "Failed to join thread since it is not joinable ";         // until the kernel starts dropping them 
            throw std::runtime_error(msg.str());
        }
        
    }
    // close all pending connections after sending an internal server error response
    HTTP_Response response;
    Router router;
    for(auto& sockfd: _pending_connections){
        response.response_code = router.getResponseCode(500);
        response.body = "";
        std::string fin_response = response.to_string();
        send(sockfd, fin_response.c_str(), fin_response.size(), 0);
        close(sockfd);
    }
}