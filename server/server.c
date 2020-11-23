// Include standard libraries
#include <unistd.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
// Include libraries for socket programming
#include <sys/socket.h> 
#include <netinet/in.h> 

#define PORT 8080 // Specify connection port
int main(int argc, char const *argv[]) 
{ 
    // Initialize variables
    int server_socket, incoming_socket, valread, incoming_addrlen; 
    struct sockaddr_in server_addr, client_addr; 
    int opt = 1; 
    //int  = sizeof(address); 
    char buffer[1024] = {0}; 
    char *hello = "Hello from server"; 
       
    // Create socket with a descriptor, specify connection type, set protocol to zero
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed");  // Socket creation error
        exit(EXIT_FAILURE); 
    } 
       
    // Connect the socket
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); // Socket attachment error
        exit(EXIT_FAILURE); 
    } 

    // Assign necessary values to socket address struct
    server_addr.sin_family = AF_INET; 
    server_addr.sin_addr.s_addr = INADDR_ANY; 
    server_addr.sin_port = htons( PORT ); 
       
    // Bind socket to port number and socket address specified in address struct
    if (bind(server_socket, (struct sockaddr *)&server_addr,  
                                 sizeof(server_addr))<0) 
    { 
        perror("bind failed"); // Socket binding error
        exit(EXIT_FAILURE); 
    } 
    if (listen(server_socket, 3) < 0)  // Set socket server to passive mode
    { 
        perror("listen");  // passive mode set error
        exit(EXIT_FAILURE); 
    } 
    if ((incoming_socket = accept(server_socket, (struct sockaddr *)&server_addr,  // grabs the first connection request from the listening socket awaiting connection queue, sockfd, establishes a new attached socket, and returns a new file descriptor that belongs to that socket.
                       (socklen_t*)&incoming_addrlen))<0) 
    { 
        perror("accept"); 
        exit(EXIT_FAILURE); 
    } 
    valread = read( incoming_socket , buffer, 1024);  // Read
    printf("%s\n",buffer ); 
    send(incoming_socket , hello , strlen(hello) , 0 );  // Send message
    printf("Hello message sent\n"); 
    return 0; 
} 