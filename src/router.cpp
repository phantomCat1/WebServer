#include "../include/router.hpp"
#include "../include/http_request.hpp"
#include<string>
#include<map>
#include <fstream>

Router::Router() {}

std::string Router::getResponseCode(int code){
    switch(code){
        case 200: return "200 OK";
        case 404: return "404 Not Found";
        case 403: return "403 Forbidden";
        case 400: return "400 Bad Request";
        case 500: return "500 Internal Service Error";
        case 501: return "501 Not Implemented";
    }
    return "404 Not Found";
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
    HTTP_Response response;
    // determine if the requested file is a regular text file or a binary
    bool isBinary = type.compare("jpeg")==0 
                    || type.compare("jpg")==0 
                    || type.compare("gif")==0 
                    || type.compare("png")==0;
    // determine opening mode of the file
    std::ios::openmode mode = isBinary ? std::ios::binary : std::ios::in;
    // try opening the file in 
    std::ifstream file("../public/" + path, mode);
    if(!file.is_open()){
        response.response_code = getResponseCode(404);
        response.body="The server couldn't fulfill the request";
        response.headers["Connection"] = "close";
    } else {
        response.response_code = getResponseCode(200);
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        response.body = content;
        response.headers["Connection"] = "close";
        response.headers["Content-Length"] = std::to_string(content.size());
        response.headers["Content-Type"] = getContentType(type);
    }
    response.http_version = "HTTP/1.0";
    return response;
}

