#ifndef SERVER
#define SERVER  
#include<string>

class HTTP_Server{
    public:
    HTTP_Server(int port): port(port){};
    ~HTTP_Server()=default;
    void start();
    void stop();

    private:
    int port;
    int server_socket;
    
    void handleClient(int client_socket);

};
#endif //SERVER