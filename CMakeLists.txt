cmake_minimum_required(VERSION 3.10)
project(WebServer VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directory
include_directories(include)

# Source files
set(SOURCES
    src/main.cpp
    src/server.cpp
    src/router.cpp
    src/http_request.cpp
)

# Create executable
add_executable(web_server ${SOURCES})

# Link with required libraries
target_link_libraries(web_server pthread)
