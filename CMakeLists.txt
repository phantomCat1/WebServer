cmake_minimum_required(VERSION 3.31)
project(WebServer)

set(CMAKE_CXX_STANDARD 17)
include_directories(include)

add_executable(http_server
src/main.cpp
src/request_handler.cpp
src/server.cpp
src/http_request.cpp)

target_link_libraries(http_server pthread)