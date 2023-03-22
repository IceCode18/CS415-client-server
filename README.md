# CS415: Client Server
This program is a client/server application developed in the C language that allows clients to request files from a server. The server will have a directory that contains files to serve to the client. Once the client requests a file, the server will send the requested file to the client.

<h3>Learning Goals</h3>
The primary learning goals of this project are:

* Create a client/server program using C language.
* Develop a protocol for the program.
* Learn how to create networked programs.

<h3>Program Description</h3>
The client/server program consists of two parts: the server and the client. The server is responsible for serving files to clients, while the client is responsible for requesting files from the server.

<h4>Server</h4>
The server opens a socket in listening mode, waiting for requests from clients. When a request is received, a new thread is launched to handle the request. The thread reads the requested file from the client and then opens the file. It determines the size of the file in bytes and sends the size to the client. After sending the size, it sends the contents of the file to the client. Once the request is completed, the thread terminates.

<h4>Client</h4>
The client requests a file from the server by connecting to it and providing the name of the file it wants to retrieve. When it receives the file from the server, it prints out what it received and quits. The client ensures that it reads the exact number of bytes that the server told it to by putting the read command in a while loop and reading until the desired number of bytes have been read.

The protocol used in this program includes a flag to indicate the end of a message. This flag is a special character used to indicate the end of the message. A newline character, $, #, etc., can be used as the flag. The client adds the flag at the end of the filename to indicate that it has read the full filename, while the server adds the flag at the end of the size to separate the size of the filename from the contents of the file.

<h3>Extra Credit</h3>
An optional extra credit task for this program is to implement thread pooling. This means having threads already launched and waiting to respond to a request. To achieve this, the server needs to be combined with the producer/consumer part of the threads lab.
