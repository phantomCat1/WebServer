#include"../include/http_request.hpp"
#include<string>
#include<sstream>

void HTTP_Request::parse(const std::string& raw_request){
    std::istringstream stream(raw_request);     // convert regular string into stream
    std::string line;

    std::getline(stream, line);                 // get first line, i.e. request line of http 
    std::istringstream request_line(line);      // convert regular request line string to a stream
    request_line >> this->method >> this->path >> this->http_version;       // separate into method, path and version

    // Parsing headers
    // Guard ensures we read each header value and makes sure we are not in the crlf before the body
    while(std::getline(stream, line) && line != "\r"){
        size_t column_pos = line.find(":");                 // find the index at which the the header key ends
        
        if(column_pos != std::string::npos){                //Check that the : charater is not the last one in the line/ header. If it is, ignore this header
            std::string key = line.substr(0, column_pos);
            std::string value = line.substr(column_pos+2);  // Skip the : and the extra space
            this->headers[key] = value;                     // Insert/update header in the map of this object
        }
    }

    //Parse the body
    std::ostringstream bodyStream;
    while(std::getline(stream, line)){
        bodyStream << line;
    }
    this->body = bodyStream.str();
}

HTTP_Response::HTTP_Response(int code, std::string http_version): http_version(http_version){
    switch(code){
        case 200: response_code = "200 OK"; break;
        case 400: response_code = "400 Bad Request"; break;
        case 404: response_code = "404 Not Found"; break;
        case 500: response_code = "500 Internal Error"; break;
        case 501: response_code = "501 Not Implemented"; break;
    }
    headers["Content-Type"] = "text/html";
    headers["Connection"] = "close";
    body = "<html><body><h1>Hello, World!</h1></body></html>";
}

std::string HTTP_Response::to_string() const{
    std::ostringstream response_stream;
    response_stream << http_version << " " << response_code << "\r\n";
    for(const auto& header: headers){
        response_stream << header.first << ": "<< header.second <<"\r\n";
    }
    response_stream<<"\r\n"<<body;
    return response_stream.str();

}

