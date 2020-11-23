// Include standard libraries
#include <stdio.h> 
#include <unistd.h> 
#include <string.h> 
#include <stdlib.h>
// Include libraries for socket programming, client side
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <sys/socket.h> 

#define BUFFER_SIZE 3000
 
int main(int argc, char const *argv[]){ 
    // Initialize variables
    int sockfd, bytes_read; 
    struct sockaddr_in server_addr;
    short port;
    char buffer[BUFFER_SIZE] = {0}; 

    
    if(argc < 2){
        printf("Enter PORT number for connection\n");
        exit(0);
    }
    port = (short)atoi(argv[1]);
    printf("Preparing client connection on Port: %d\n", port);
    
    // Create socket with a descriptor, specify connection type, set protocol to zero
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){ 
        perror("Socket creation error"); 
        exit(0); 
    } 
    bzero(&server_addr, sizeof(server_addr));

    
    // Assign necessary values to server socket address struct
    server_addr.sin_family = AF_INET; 
    server_addr.sin_port = htons(port); 
       

    //Parse text type IP addresses to convert to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) { 
        perror("Invalid address, Address not supported"); 
        exit(0);
    } 


    //Connect to remote server
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) != 0){ 
        perror("Connection failed"); 
        exit(0); 
    } 

    
    bytes_read = read(sockfd, buffer, sizeof(buffer));
    buffer[bytes_read] = 0;
    printf("Server: %s\n", buffer);
    bzero(&buffer, sizeof(buffer));
    
    char filename[BUFFER_SIZE];
    scanf("%s", filename);
    
    strcpy(buffer, filename);
    write(sockfd, buffer, strlen(buffer));
    bzero(&buffer, sizeof(buffer));
    
} 

// char filename[100];

//     printf( "Enter a value :");
//     scanf("%s %d", filename, &i);
//     printf( "\nYou entered: %s %d ", str, i);

   
    
//     bytes_read = read(sockfd, buffer, sizeof(buffer));
//     buffer[bytes_read] = 0;
//     printf("From the server: %s\n", buffer);
    
//     sprintf(buffer, "I am sending this message to the server.");
//     write(sockfd, buffer, strlen(buffer));
//     bzero(&buffer, sizeof(buffer));