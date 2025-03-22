#ifndef HTTP_MESSAGE
#define HTTP_MESSAGE

#include<string>
#include<unordered_map>

class HTTP_Request{
    public:
    HTTP_Request(): http_version("HTTP/1.0"){};
    ~HTTP_Request()=default;
    void parse(const std::string& raw_request);
    std::string http_version;
    std::string path;
    std::string method;
    std::unordered_map<std::string,std::string> headers;
    std::unordered_map<std::string,std::string> query_param;
    std::string body;
    
};

class HTTP_Response{
    public:
    HTTP_Response();
    ~HTTP_Response()=default;
    std::string to_string() const;
    std::string http_version;
    std::string response_code;
    std::unordered_map<std::string,std::string> headers;
    std::string body;
};
#endif //HTTP_REQ