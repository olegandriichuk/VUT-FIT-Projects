# **IPK Project 1**
## Description

The project is an implementation of a client for simple calculator server. The client and server can communicate via TCP or UDP. The protocol is described in the assignment repository.


## Usage

The client can be compiled using `make` command. The binary is called `ipk24chat-client`. The client can be run using `./ipk24chat-client` command. The client should be run with the following arguments:

- `-t` - Transport protocol used for connection ('tcp' or 'udp')
- `-s` - Server IP or hostname
- `-p` - Server port
- `-d` - UDP confirmation timeout
- `-r` - Maximum number of UDP retransmissions
- `-h` - Prints program help output and exits

## Implementation
The client is implemented in C++ in the C++20 standard. Project contains only implementation of TCP protocol.  
The main source code is main.cpp. Class.cpp is created for Client class. 
Other files are auxiliary for the correct operation of the chat, in particular, the Connection.cpp file is used for authentication, sending messages, and logging in to different channels.  
The constructMessages.cpp file contains auxiliary functions for sending and receiving recycled messages. DataValidation.cpp contains simple functions, which valdite main message parametrs.
The most important file after main.cpp is Client.cpp, where we create Client object.

## Main function
As it was written before, the main function is main.cpp 
This file represents a main application for a chat client that communicates with a server using TCP or UDP protocols, based on user specifications. 
The program starts by parsing command-line arguments to configure its settings, such as the protocol type, hostname, port, timeout, and retransmission count. 
These settings are used to initialize and manage a client instance that connects to a server to send and receive messages. 
In this code, the use of epoll is a central feature that enables the chat client to efficiently manage multiple input sources simultaneously. 
Epoll is a scalable I/O event notification system, specific to Linux, that allows applications to monitor multiple file descriptors to see if I/O operations are possible on any of them.
The application utilizes the epoll API for efficient event handling, allowing it to simultaneously listen for input from the server and the user (via stdin).
This capability is what allows the application to function as an interactive chat client, capable of real-time communication with a server while also processing user commands seamlessly.
The program runs in a while loop, where we use states from the finite state machine and move through them depending on our actions. Responses from the server are received 
using function "receiveFromServer(char* buffer)" and are printed to the user. User sends messages to the server using function sendClient(const char* buffer).

## TCP client
The TCP client is being created, connected and disconnected in Client.cpp source file. The TCP client first finds the server using the gethostbyname() function.
Then we set up a TCP connection for a client, starting with protocol specification and hostname resolution. 
Upon successfully resolving the hostname, it creates a TCP socket for IPv4 communication and verifies its creation. 
The server's address details are then set, including converting the port number to network byte order. The client attempts to connect to the server using the configured socket. 
If the connection fails, it reports an error and closes the socket. Once connected, the client can communicate with the server.
Upon completion of its operations, the client cleanly disconnects by closing the epoll file descriptor, shutting down the socket to cease all data transmission, and finally closing the socket to free up resources. 
This ensures a graceful termination of the connection.

## Testing 
The client was tested on Ubuntu 22.04. Testing was made by hand using mainly netcat. Many different cases were tested using own pseudo-server. They will be shown as screenshots in a folder "tests."

##References
[Task of the 1 project](https://git.fit.vutbr.cz/NESFIT/IPK-Projects-2024/src/branch/master/Project%201)
[epoll(7) — Linux manual page](https://man7.org/linux/man-pages/man7/epoll.7.html)
