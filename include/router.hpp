#ifndef ROUTER
#define ROUTER

#include<string>
#include"http_request.hpp"

class Router{
    public:
    Router();
    ~Router()= default;
    std::string getContentType(const std::string& type);
    std::string getResponseCode(int code);
    HTTP_Response getResource(const std::string& path, const std::string& type);
};
#endif