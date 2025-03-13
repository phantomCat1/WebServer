#include<string>
#include<map>
#include "../include/router.hpp"
#include "../include/http_request.hpp"
std::string Router::getResponseCode(int code){
    switch(code){
        case 200: return "200 OK";
        case 404: return "404 Not Found";
        case 403: return "403 Forbidden";
        case 400: return "400 Bad Request";
        case 500: return "500 Internal Service Error";
        case 501: return "501 Not Implemented";
    }
}

std::string Router::getContentType(const std::string& type){
    static const std::map<std::string, std::string> contenttype = {
        {"html", "text/html"},
        {"css", "text/css"},
        {"plain", "text/plain"},
        {"js", "text/javascript"},
        {"jpeg", "image/jpeg"},
        {"png", "image/png"},
        {"jpg", "image/jpg"},
        {"gif", "image/gif"},
        {"json", "application/json"},
        {"pdf", "application/pdf"},
        {"xml", "application/xml"}
    };
    auto it = contenttype.find(type);
    if(it != contenttype.end()){
        return it->second;
    } else {
        return "application/octet-stream";
    }
}

HTTP_Response Router::getResource(const std::string& path, const std::string& type){
    
}

