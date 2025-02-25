#include<iostream>
#include<sys/socket.h>
#include<thread>
#include<string>
#include "../include/server.hpp"
#include "../include/http_request.hpp"
#include<thread>

int main(int argc, char* argv[]){
    std::uint32_t port = 8080;
    HTTP_Server server(port);
    try{
        std::cout << "Starting server...\n";
        server.start();
        std::cout << "Enter [stop] to stop the web server\n";
        std::string command;
        while(std::cin >> command, command != "stop") {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        std::cout<<"Stopping web server...\n";
        //server.stop();
        std::cout<< "Server stopped successfully\n";
    } catch(std::exception e) {
        std::cout<< "Error occured while starting server\n";
        return -1;
    }

    
    

    return 0;
}