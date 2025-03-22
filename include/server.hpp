#ifndef SERVER
#define SERVER  
#include<string>
#include<vector>
#include<mutex>
#include<thread>
#include<condition_variable>
#include<atomic>
#include<deque>

class HTTP_Server{
    public:
    HTTP_Server(int port): _port(port){};
    ~HTTP_Server()=default;
    void start();
    void stop();

    private:
    int _port;
    int _server_socket;
    const int MAX_WORKERS = 10;
    const int MAX_PENDING_CONN = 20;
    std::atomic<bool> _ended {false};
    std::deque<int> _pending_connections;
    std::vector<std::thread> _workers;
    std::mutex _request_mutex;
    std::condition_variable cv;
    void createListeningThread();
    void handleClient();

};
#endif //SERVER