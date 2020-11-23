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
    int server_fd, new_socket, valread; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    char buffer[1024] = {0}; 
    char *hello = "Hello from server"; 
       
    // Create socket with a descriptor, specify connection type, set protocol to zero
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed");  // Socket creation error
        exit(EXIT_FAILURE); 
    } 
       
    // Connect the socket forcefully to port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); // Socket attachment error
        exit(EXIT_FAILURE); 
    } 

    // Assign necessary values to socket address struct
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
       
    // Bind socket to port number and socket address specified in address struct
    if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        perror("bind failed"); // Socket binding error
        exit(EXIT_FAILURE); 
    } 
    if (listen(server_fd, 3) < 0)  // Set socket server to passive mode
    { 
        perror("listen");  // passive mode set error
        exit(EXIT_FAILURE); 
    } 
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  // grabs the first connection request from the listening socket awaiting connection queue, sockfd, establishes a new attached socket, and returns a new file descriptor that belongs to that socket.
                       (socklen_t*)&addrlen))<0) 
    { 
        perror("accept"); 
        exit(EXIT_FAILURE); 
    } 
    valread = read( new_socket , buffer, 1024);  // Read
    printf("%s\n",buffer ); 
    send(new_socket , hello , strlen(hello) , 0 );  // Send message
    printf("Hello message sent\n"); 
    return 0; 
} 