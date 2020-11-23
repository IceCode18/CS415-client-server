// Include standard libraries
#include <unistd.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
// Include libraries for socket programming
#include <sys/socket.h> 
#include <sys/types.h>
#include <netinet/in.h> 


int main(int argc, char const *argv[]) { 
    
    
    // Initialize variables
    int server_socket, incoming_socket, valread, incoming_addrlen; 
    struct sockaddr_in server_addr, client_addr; 
    short port;
    
    
    if(argc < 2){
        printf("Enter PORT number for connection\n");
        exit(0);
    }
    port = (short)atoi(argv[1]);
    printf("Preparing server connection on Port: %d\n", port);
    
    
    // Create socket with a descriptor, specify connection type, set protocol to zero
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1){ 
        perror("Socket creation error"); 
        exit(0); 
    } 
    bzero(&server_addr, sizeof(server_addr));


    // Assign necessary values to socket address struct
    server_addr.sin_family = AF_INET; 
    server_addr.sin_addr.s_addr = INADDR_ANY; 
    server_addr.sin_port = htons(port); 
       
       
    // Bind socket to port number and socket address specified in address struct
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){ 
        perror("Socket binding failed"); // Socket binding error
        exit(0); 
    } 
    print("Socket binding successful...")
    
    
    // Set socket server to passive mode 
    if (listen(server_socket, 3) != 0){  
        perror("Listening mode error");  
        exit(0); 
    }
    incoming_addrlen = sizeof(client_addr)
    
    
    while(1){
        // grabs the connection request from the listening socket awaiting connection queue, sockfd, establishes a new attached socket, and returns a new file descriptor that belongs to that socket.
        if ((incoming_socket = accept(server_socket, (struct sockaddr*)&client_addr, &incoming_addrlen)) < 0){ 
            perror("Accepting new connection failed"); 
            exit(0); 
        }
        printf("New connection accepted\n");
    }
    
     
    // valread = read( incoming_socket , buffer, 1024);  // Read
    // printf("%s\n",buffer ); 
    // send(incoming_socket , hello , strlen(hello) , 0 );  // Send message
    // printf("Hello message sent\n"); 
    // return 0; 
} 