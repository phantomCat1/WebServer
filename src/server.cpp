#include "../include/server.hpp"
#include"../include/http_request.hpp"
#include<sys/socket.h>
#include<netdb.h>
#include<sys/types.h>
#include<unistd.h>
#include<iostream>
#include<thread>
#include<netinet/in.h>
#include<sstream>

void HTTP_Server::start(){
    server_socket = socket(AF_INET, SOCK_STREAM, 0);        // Create a socket using TCP (by SOCK_STREAM) and IPv4 (by AF_INET)
    if(server_socket == -1){                                
        std::cerr<<"Error creating socket\n";               // Handle error case if socket creation fails
        return;
    }

    sockaddr_in server_addr{};                              // Create struct which will store information about the internet address
    server_addr.sin_family = AF_INET;                       // Fill in values about the address: first is that it uses IPv4
    server_addr.sin_port = htons(port);                     // Next we give the port, which converts the integer port to network byte order
    server_addr.sin_addr.s_addr = INADDR_ANY;               // Basically give it any available IP adress on the system
    
    if(bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){   
        std::cerr<<"Error binding\n";                       // Try to bind the socket and output error message if failed
        return;
    }

    if (listen(server_socket, 10) < 0) {                    // Listen for connection requests, while having a max queue of 10 pending requests
        std::ostringstream msg;
        msg << "Failed to listen on port " << port;         // until the kernel starts dropping them 
        throw std::runtime_error(msg.str());
      }                             
                                                            
    std::cout<<"Server listening on port: "<<port<<"\n";
    while(true){
        int client_socket = accept(server_socket, nullptr, nullptr);
        if(client_socket > 0){
            std::thread(&HTTP_Server::handleClient, this, client_socket).detach();    
        }
    }
}

void HTTP_Server::handleClient(int client_socket){
    char buffer[4096]={0};
    int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);

    if(bytes_received > 0) {
        std::string request_str(buffer, bytes_received);
        HTTP_Request http_request;
        http_request.parse(request_str);

        HTTP_Response http_response;
        std::string response_str = http_response.to_string();
        send(client_socket, response_str.c_str(), response_str.size(), 0);

    }
    close(client_socket);
}