#include<stdio.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>


// Create listening socket using UNIX socket interface
// bind socket
// listen
// if connection is trying to be established, create separate socket in different thread
// each thread handles communication with their particular client
// implement HTTP protocol
// 