#include<iostream>
#include<sys/socket.h>
#include<thread>
#include<string>
#include "../include/server.hpp"
#include "../include/http_request.hpp"

int main(int argc, char* argv[]){
    std::string ip_address = "";
    std::uint32_t port = 8080;
    //declare server object with ip_address and port
    //start new thread in which we start the server
    // try{
    //     std::cout << "Starting server...\n";
    //     std::cout << "Enter [stop] to stop the web server\n";
    //     std::string command;
    //     while(std::cin >> command, command != "stop") {

    //     }
    //     //stop server
    //     std::cout<< "Server stopped successfully\n";
    // } catch(std::exception e) {
    //     std::cout<< "Error occured while starting server\n";
    //     return -1;
    // }
    HTTP_Server server(port);
    server.start();
    
    

    return 0;
}