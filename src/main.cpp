#include<iostream>
#include<sys/socket.h>
#include<string>
#include "../include/server.hpp"

int main(int argc, char* argv[]){
    int port = 8080;
    HTTP_Server server(port);
    try{
        std::cout << "Starting server...\n";
        server.start();
        std::cout << "Enter [stop] to stop the web server"<<std::endl;
        std::string command;
        while(std::cin >> command, command.compare("stop")!=0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        std::cout<<"Stopping web server...\n";
        server.stop();
        std::cout<< "Server stopped successfully\n";
    } catch(std::exception e) {
        std::cout<< "Error occured while starting server:"<<e.what()<<std::endl;
        server.stop();
        return -1;
    }

    
    

    return 0;
}