// Include standard libraries
#include <unistd.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <pthread.h>
#include <semaphore.h>
// Include libraries for socket programming
#include <sys/socket.h> 
#include <sys/types.h>
#include <netinet/in.h> 



// Functions
void handle_request(int socketfd);
void message(int socketfd, char data[]);
void send_file(int socketfd, char filename[]);

#define BUFFER_SIZE 3000
short port;

int main(int argc, char const *argv[]) { 
    // Initialize variables
    int server_socket, incoming_socket, incoming_addrlen; 
    struct sockaddr_in server_addr, client_addr; 
    
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
    printf("Socket binding successful...\n");
    
    
    // Set socket server to passive mode 
    if (listen(server_socket, 3) != 0){  
        perror("Listening mode error");  
        exit(0); 
    }
    incoming_addrlen = sizeof(client_addr);
    
    while(1){
        // Accepting Requests
        if ((incoming_socket = accept(server_socket, (struct sockaddr*)&client_addr, &incoming_addrlen)) < 0){ 
            perror("Accepting new connection failed"); 
            exit(0); 
        }else{
            handle_request(incoming_socket);
        }
    }
    
} 



void handle_request(int socketfd){
    char filename[BUFFER_SIZE];
    int bytes_read = 0;
    
    message(socketfd, "Hello Client! What file do you want to receive?");
    
    bytes_read = read(socketfd, filename, BUFFER_SIZE);
    filename[bytes_read] = 0;
    printf("Client wants file (%d): %s\n", bytes_read, strtok(filename, "$"));
    
    send_file(socketfd, filename);
    
}

void message(int socketfd, char data[]){
    char message[BUFFER_SIZE];
    sprintf(message, "%s", data);
    write(socketfd, message, strlen(message));
}

void send_file(int socketfd, char filename[]){
    char path[BUFFER_SIZE] = "./root/";
    char header[BUFFER_SIZE];
    int data_size;
    FILE *fp;
    
    // Open File
    strcat(path, filename);
    fp = fopen(path, "r" );
    if (fp == NULL){ 
        perror("Cannot open file"); 
        exit(0); 
    } 
    printf("Opening %s \n", path);
    
    // Calculating the size of the file 
    fseek(fp, 0L, SEEK_END);
    data_size = ftell(fp);
    fclose(fp);
    printf("File size is %d bytes\n", data_size);
    
    // Add flag to file size
    sprintf(header, "%d$", data_size);
    printf("Sending: %s\n", header);
    message(socketfd, header);
    
    // Read file content
    printf("Sending: (%d:%d): ", data_size, data_size);
    fp = fopen(path, "r" );
    char data[data_size];
    char c = fgetc(fp); 
    for(int i; i < data_size; i++){
        printf ("%c", c); 
        data[i] = c;
        c = fgetc(fp);
    }
    fclose(fp); 
    
    printf("\n"); 
    message(socketfd, data);
    
    printf("File data sent successfully.\n");
}

