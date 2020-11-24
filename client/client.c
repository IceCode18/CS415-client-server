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
    strcat(buffer, "$");
    write(sockfd, buffer, strlen(buffer));
    bzero(&buffer, sizeof(buffer));
    
    char payload_bucket[BUFFER_SIZE];
    int payload_size;
    char *flag;
    int flagFound = 0;
    int count = 0;
    while(1){
        if(count > BUFFER_SIZE ){
            break;
        }
        count++;
        //read
        bytes_read = read(sockfd, buffer, sizeof(buffer));
        buffer[bytes_read] = 0;
        strncpy(payload_bucket, buffer, strlen(buffer));
        bzero(&buffer, sizeof(buffer));
        // check for flag and parse file content size
        if(flagFound < 1){
            flag = strchr(payload_bucket, '$');
            if( flag != NULL){
                int payload_size_header = strlen(payload_bucket) - strlen(flag);
                char payload_size_buffer[BUFFER_SIZE/2];
                for(int i = 0; i< payload_size_header; i++){
                    payload_size_buffer[i] = payload_bucket[i];
                }
                payload_size = atoi(payload_size_buffer);
                printf("File size: %d\n", payload_size);
                flagFound++;
            }
        }
        // Check if payload buffer size is equal to the file content size then print payload buffer
        if(strlen(payload_bucket) >= payload_size){
            printf("File content: %s\n", payload_bucket);
            break;
        }
    }
    printf("\n");
    close(sockfd);
    
} 
