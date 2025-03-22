#include"../include/http_request.hpp"
#include<string>
#include<sstream>

void HTTP_Request::parse(const std::string& raw_request){
    std::istringstream stream(raw_request);     // convert regular string into stream
    std::string line;

    std::getline(stream, line);                 // get first line, i.e. request line of http 
    std::istringstream request_line(line);      // convert regular request line string to a stream
    std::string path_with_param;
    request_line >> this->method >> path_with_param >> this->http_version;       // separate into method, path and version

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

    // Parse query parameters which we will ignore
    std::istringstream path_stream(path_with_param);
    std::getline(path_stream, this->path, '?');         // Extract path to resource
    std::string pair;                                   // Used to store each parameter, value pair
    while(std::getline(path_stream, pair, '&')){        // Read until no more parameters are found
        size_t pos_equal = pair.find('=');              // Find delimiting = char between the param and value
        if(pos_equal != std::string::npos){             // Checks that there is some value after =
            std::string key = pair.substr(0,pos_equal);
            std::string value = pair.substr(pos_equal+1);
            this->query_param[key] = value;
        }
    }
}

HTTP_Response:: HTTP_Response(){}
std::string HTTP_Response::to_string() const{
    std::ostringstream response_stream;
    response_stream << http_version << " " << response_code << "\r\n";
    for(const auto& header: headers){
        response_stream << header.first << ": "<< header.second <<"\r\n";
    }
    response_stream<<"\r\n"<<body;
    return response_stream.str();

}

