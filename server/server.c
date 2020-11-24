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
void *producer_job(void *arg);
void *consumer_job(void *arg);
void insert(int id, int value);
int take(int id);
void handle_request(int socketfd);
void message(int socketfd, char data[]);
void send_file(int socketfd, char filename[]);

// Thread pooling variables
int buffer_count;
sem_t s_lock, n_lock, e_lock;
int *buffer;

#define BUFFER_SIZE 3000
short port;

int main(int argc, char const *argv[]) { 
    // Initialize thread pooling variables
    int total_threads;
    pthread_t *threads_array;
    buffer_count = 0;
    const int MAX_SOCKETS = 3;
    
    if(argc < 2){
        printf("Enter PORT number for connection\n");
        exit(0);
    }
    port = (short)atoi(argv[1]);
    printf("Preparing server connection on Port: %d\n", port);
    
   
    // Initialize number of threads and threads array
    total_threads = 4;
    threads_array = (pthread_t*) calloc(total_threads,sizeof(pthread_t));
    buffer = (int*) calloc(MAX_SOCKETS, sizeof(int));

    // Initialize semaphores for Part C
    if(sem_init(&s_lock,0,1) < 0){
        perror("Error initializing buffer_lock_S");
        return 0;
    }
    if(sem_init(&n_lock,0,0) < 0){
        perror("Error initializing buffer_lock_N");
        return 0;
    }
    if(sem_init(&e_lock,0,MAX_SOCKETS) < 0){
        perror("Error initializing buffer_lock_E");
        return 0;
    }


    // Launch threads
    printf("Launching a total of %d threads... ( 1 producer and %d consumers )\n", total_threads, total_threads-1);
    for(int i=0; i<total_threads; i++){
        long thread_id = (long)i;
        if( i == 0){
            if(pthread_create(&threads_array[i], NULL, &producer_job,(void *)thread_id) != 0){
                perror("Error");
            }
        }else{
            if(pthread_create(&threads_array[i], NULL, &consumer_job,(void *)thread_id) != 0){
                perror("Error");
            }
        }
        
    }

    // Prevent threads from terminating early
    for(int i=0; i<total_threads; i++){
        pthread_join(threads_array[i], NULL);
    }
    
    

    free(threads_array);
    free(buffer);
    
    sem_destroy(&s_lock);
    sem_destroy(&n_lock);
    sem_destroy(&e_lock);
    
    
} 

// Producer Thread Job
void *producer_job(void *arg){
    int t_id = (int)(long)arg;
    printf("Producer %d launched . . . \n", t_id);
    
    // Initialize variables
    int server_socket, incoming_socket, incoming_addrlen; 
    struct sockaddr_in server_addr, client_addr; 
    
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
            sem_wait(&e_lock);
            sem_wait(&s_lock);
            printf("Producer %d attempting to add Socket %d to buffer\n", t_id, incoming_socket); //produce
            insert(t_id, incoming_socket); //append
            sem_post(&s_lock);
            sem_post(&n_lock);
        }
        
    }
}

// Consumer Thread Job
void *consumer_job(void *arg){
    int t_id = (int)(long)arg;
    printf("Consumer %d launched . . . \n", t_id);
    while(1){
        sem_wait(&n_lock);
        sem_wait(&s_lock);
        int socket = take(t_id); //take
        sem_post(&s_lock);
        sem_post(&e_lock);
        printf("Consumer %d handled socket %d successfully.\n", t_id, socket); //consume
    }
}

// Insert Operation
void insert(int id, int value){
    buffer[buffer_count] = value;
    buffer_count++;
    printf("Producer %d successfully added socket %d to buffer. Buffer count: %d\n", id, value, buffer_count);
    

}
// Take Operation
int take(int id){
    int socket = buffer[buffer_count-1];
    printf("Consumer %d starting to handle socket %d from buffer. Buffer count: %d\n", id, socket, buffer_count); 
    handle_request(socket);
    buffer_count--;
    return socket;
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
    close(socketfd);
}

