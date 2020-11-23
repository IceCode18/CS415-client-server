// Include standard libraries
#include <stdio.h> 
#include <unistd.h> 
#include <string.h> 
// Include libraries for socket programming, client side
#include <sys/socket.h> 
#include <arpa/inet.h> 

#define PORT 8080 // Specify connection port
   
int main(int argc, char const *argv[]) 
{ 
    // Initialize variables
    int sock = 0, valread; 
    struct sockaddr_in serv_addr; 
    char *hello = "Hello from client"; 
    char buffer[1024] = {0}; 

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) // Create socket with a descriptor, specify connection type, set protocol to zero
    { 
        printf("\n Socket creation error \n"); // Socket creation error
        return -1; 
    } 
   
    // Assign necessary values to server socket address struct
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
       
    //Parse text type IP addresses to convert to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); // Invalid address error
        return -1; 
    } 
    //Connect to remote server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    } 
    send(sock , hello , strlen(hello) , 0 ); // Send message
    printf("Hello message sent\n"); 
    valread = read( sock , buffer, 1024);  // Read
    printf("%s\n",buffer ); 
    return 0; 
} 