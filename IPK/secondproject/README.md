# IPK24 Project2: Chat server
The project is an implementation of a server for simple communication with clients. The client and server can communicate via TCP or UDP. The protocol is described in the assignment repository.

## Table of Contents

- [IPK24 Project2: Chat server](#IPK24-CHAT-client-documentation)
    - [Table of Contents](#table-of-Contents)
    - [Introducing](#introducing)
    - [Including files](#including-files)
    	- [main.cpp](#maincpp)
        	- [Command Line Arguments](#command-line-arguments)
        	- [Server Initialization](#server-initialization)
        	- [Connection Handling](#connection-handling)
        	- [Data Handling](#data-handling)
        	- [Signal Handling](#signal-handling)
        	- [Resource Cleanup](#resource-cleanup)
        - [Server.cpp](#servercpp)
        - [tcpClient.cpp](#tcpclientcpp)
        - [interrupt.cpp](#interruptcpp) 
    - [Usage](#usage)
        - [Command-Line Arguments](#command-line-arguments-1)
        - [Receiving Messages](#receiving-messages)
        - [Sending replies](#sending-replies)
        - [Exiting the Program](#exiting-the-program)
    - [Testing](#testing)
        - [Data Transfer testing](#data-transfer-testing)
            - [Objective](#objective)
            - [Testing in wireshark](#testing-in-wireshark)
    - [Bibliography](#bibligraphy)   
 
## Introducing 

This project's task is to design and implement a server application capable of communicating with remote clients using the IPK24-CHAT protocol. The protocol has got two variants - each built on top of a different transport protocol. The server allows clients to receive and send messages to other clients, join different channels, and terminate the connection using the C-c signal.

## Including files

The server is implemented in C++ in the C++20 standard. Project contains only implementation for communication with clients with TCP protocol. Below is a summary of the primary components of the IPK_Server application:

### main.cpp

main.cpp file is the main part of a server application designed to handle multiple client connections using the epoll mechanism on a Linux system. It initializes a server on a specified port and listens for incoming TCP connections. Below are the key functionalities implemented in this file:
#### Command Line Arguments

The server can be customized through command-line options specifying hostname (-l), port (-p), timeout (-d), and retransmission attempts (-r). The server displays usage information with -h.

#### Server Initialization

It sets up a server with the specified hostname and port. If the epoll_create1 call fails, it handles the error gracefully by outputting an appropriate message and exiting.
#### Connection Handling

The server listens for incoming connections. For each new connection, it sets the socket to non-blocking mode and adds it to the epoll instance to manage multiple connections efficiently.
#### Data Handling

For each active client socket that is ready to read data (indicated by EPOLLIN), the server reads the buffer, processes commands (e.g., "AUTH", "JOIN", "MSG", "ERR", "BYE"), and changes the client's state accordingly.
Client State Management: Based on the command parsed from the client's message, the server updates the client's state (e.g., from "Accept" to "Open" or "Auth", from "Open" to "End" or "Error") and sends appropriate responses back to the client.
#### Signal Handling 

The server uses sigaction to handle SIGINT (Ctrl+C). It cleans up by sending "BYE" to all connected clients, closing sockets, and exiting the application.
#### Resource Cleanup

On receiving a shutdown signal or in case of an error, the server gracefully disconnects all clients, releases resources, and terminates.

### Server.cpp

Makes clients sockets non-blocking. Receives messages from clients and send them to other clients. Also removes clients from server when they are int the state "End".
 
### tcpClient.cpp

The tcpClient class is designed to encapsulate the state and operations associated with a single TCP client within a server application.

### serverReplies.cpp 

This source file defines a set of utility functions for constructing server reply messages and handling string manipulations related to TCP client interactions.These functions are essential for sending formatted responses and notifications to clients connected to a TCP server, particularly in applications involving user authentication, channel management, and general messaging functionality. The functions ensure that all messages are safely copied into buffers with proper null-termination to prevent buffer overflow vulnerabilities.


### interrupt.cpp

This source file provides the implementation for handling the SIGINT signal (Ctrl+C), aiming to gracefully manage the shutdown process.

## Class diagram
This is how Class Diagram of this program should be looking

![Class Diagram](/Class_diagram.png)

## Usage 

###  Command-Line Arguments

The server should be compiled using `make` command. The binary is called `ipk24chat-server`. The server can be run using `./ipk24chat-server` command. The client should be run with the following arguments:
- -l		Server listening IP address for welcome sockets
- -p		Server listening port for welcome sockets
- -d		UDP confirmation timeout
- -r		Maximum number of UDP retransmissions
- -h		Prints program help output and exits

### Receiving Messages
The application listens for messages, authentication and join requests from the clients and displays them in a specific format:
- RECV {FROM_IP}:{FROM_PORT} | {MESSAGE_TYPE}[MESSAGE_CONTENTS]\n

### Sending replies
The application sends replies to clients when they want to authenticate or to join channels and displays replies in a specific format:
- SENT {TO_IP}:{TO_PORT} | {MESSAGE_TYPE}[MESSAGE_CONTENTS]\n

### Exiting the Program

The server can shut down and disconnect all connected clients when ctrl+c is pressed. In that moment all client's sockets will be closed and listening socket also. 

## Testing

The server was tested on Ubuntu 22.04. Testing was made by hand using  project 1 chat client. Testing was mainly provided by using 4-5 clients which were connected to my server. During testing these things were tested:
 - Client-side termination using ctrl+c  
 - Ending communication with clients using ctrl + c
 - Joining channels 
 - Receiving messages from clients and sending them to other clients
 - Case insensitivity in case of provided ABNF [RFC5234] grammar

### Data Transfer testing 

Data transfer assessments are crucial checks designed to validate the efficiency and precision of data movement within a software system or across network connections. These evaluations focus on confirming the integrity, performance, and dependability of data as it is transmitted from one location to another, whether this involves intra-system communication, local network exchanges, or transfers over the internet.
#### Objective
The primary objectives of data transfer assessments include:
Guaranteeing Data Accuracy: Ensure that data transmitted from the source to the destination remains unaltered and correct.
Measuring Transfer Efficiency: Determine the speed and effectiveness of the data transfer process, pinpointing potential slowdowns or inefficiencies.
Confirming Transfer Dependability: Test the reliability of data transfer methods in various scenarios, such as during peak usage times or in suboptimal network conditions.

Data transfer was tested by using chat-client from 1 projects. Screenshots from testing are in test.pdf. 
This is a file which contains testing. 
![Testing](/Testing.pdf)
Below there is a testing using wireshark.

#### Testing in wireshark 

	Source		Destination	Protocol	Length	Info
	127.0.0.1	127.0.0.1	IPK24-CHAT	87	C → Server | aUtH 1 aS 4 uSinG 2
	127.0.0.1	127.0.0.1	IPK24-CHAT	92	Server → C | REPLY OK IS AUTH SUCCESS
	127.0.0.1	127.0.0.1	IPK24-CHAT	107	Server → C | MSG FROM Server IS 4 has joined default
	127.0.0.1	127.0.0.1	IPK24-CHAT	87	C → Server | msG FRoM 4 iS hello
	127.0.0.1	127.0.0.1	IPK24-CHAT	92	C → Server | aUtH wow aS wow uSinG 22
	127.0.0.1	127.0.0.1	IPK24-CHAT	92	Server → C | REPLY OK IS AUTH SUCCESS
	127.0.0.1	127.0.0.1	IPK24-CHAT	109	Server → C | MSG FROM Server IS wow has joined default
	127.0.0.1	127.0.0.1	IPK24-CHAT	109	Server → C | MSG FROM Server IS wow has joined default
	127.0.0.1	127.0.0.1	IPK24-CHAT	89	C → Server | msG FRoM wow iS hello
	127.0.0.1	127.0.0.1	IPK24-CHAT	89	Server → C | msG FRoM wow iS hello
	127.0.0.1	127.0.0.1	IPK24-CHAT	90	C → Server | msG FRoM akar iS hello
	127.0.0.1	127.0.0.1	IPK24-CHAT	90	Server → C | msG FRoM akar iS hello
	127.0.0.1	127.0.0.1	IPK24-CHAT	94	C → Server | aUtH ewe aS ewe uSinG 5678
	127.0.0.1	127.0.0.1	IPK24-CHAT	92	Server → C | REPLY OK IS AUTH SUCCESS
	127.0.0.1	127.0.0.1	IPK24-CHAT	109	Server → C | MSG FROM Server IS ewe has joined default
	127.0.0.1	127.0.0.1	IPK24-CHAT	109	Server → C | MSG FROM Server IS ewe has joined default
	127.0.0.1	127.0.0.1	IPK24-CHAT	109	Server → C | MSG FROM Server IS ewe has joined default
	127.0.0.1	127.0.0.1	IPK24-CHAT	88	C → Server | jOIn channel1 aS ewe
	127.0.0.1	127.0.0.1	IPK24-CHAT	92	Server → C | REPLY OK IS JOIN SUCCESS
	127.0.0.1	127.0.0.1	IPK24-CHAT	107	Server → C | MSG FROM Server IS ewe has left default
	127.0.0.1	127.0.0.1	IPK24-CHAT	107	Server → C | MSG FROM Server IS ewe has left default
	127.0.0.1	127.0.0.1	IPK24-CHAT	110	Server → C | MSG FROM Server IS ewe has joined channel1
	127.0.0.1	127.0.0.1	IPK24-CHAT	104	C → Server | msG FRoM ewe iS hello in new channel
	127.0.0.1	127.0.0.1	IPK24-CHAT	100	C → Server | aUtH palop aS palop uSinG qwerty
	127.0.0.1	127.0.0.1	IPK24-CHAT	92	Server → C | REPLY OK IS AUTH SUCCESS
	127.0.0.1	127.0.0.1	IPK24-CHAT	111	Server → C | MSG FROM Server IS palop has joined default
	127.0.0.1	127.0.0.1	IPK24-CHAT	111	Server → C | MSG FROM Server IS palop has joined default
	127.0.0.1	127.0.0.1	IPK24-CHAT	111	Server → C | MSG FROM Server IS palop has joined default
	127.0.0.1	127.0.0.1	IPK24-CHAT	100	C → Server | msG FRoM palop iS hello everyone
	127.0.0.1	127.0.0.1	IPK24-CHAT	100	Server → C | msG FRoM palop iS hello everyone
	127.0.0.1	127.0.0.1	IPK24-CHAT	100	Server → C | msG FRoM palop iS hello everyone
	127.0.0.1	127.0.0.1	IPK24-CHAT	90	C → Server | jOIn channel1 aS palop
	127.0.0.1	127.0.0.1	IPK24-CHAT	92	Server → C | REPLY OK IS JOIN SUCCESS
	127.0.0.1	127.0.0.1	IPK24-CHAT	109	Server → C | MSG FROM Server IS palop has left default
	127.0.0.1	127.0.0.1	IPK24-CHAT	109	Server → C | MSG FROM Server IS palop has left default
	127.0.0.1	127.0.0.1	IPK24-CHAT	112	Server → C | MSG FROM Server IS palop has joined channel1
	127.0.0.1	127.0.0.1	IPK24-CHAT	112	Server → C | MSG FROM Server IS palop has joined channel1
	127.0.0.1	127.0.0.1	IPK24-CHAT	113	C → Server | msG FRoM palop iS hello who is in this server
	127.0.0.1	127.0.0.1	IPK24-CHAT	113	Server → C | msG FRoM palop iS hello who is in this server
	127.0.0.1	127.0.0.1	IPK24-CHAT	87	C → Server | jOIn default aS ewe
	127.0.0.1	127.0.0.1	IPK24-CHAT	92	Server → C | REPLY OK IS JOIN SUCCESS
	127.0.0.1	127.0.0.1	IPK24-CHAT	108	Server → C | MSG FROM Server IS ewe has left channel1
	127.0.0.1	127.0.0.1	IPK24-CHAT	109	Server → C | MSG FROM Server IS ewe has joined default
	127.0.0.1	127.0.0.1	IPK24-CHAT	109	Server → C | MSG FROM Server IS ewe has joined default
	127.0.0.1	127.0.0.1	IPK24-CHAT	109	Server → C | MSG FROM Server IS ewe has joined default
	127.0.0.1	127.0.0.1	IPK24-CHAT	89	C → Server | jOIn default aS palop
	127.0.0.1	127.0.0.1	IPK24-CHAT	92	Server → C | REPLY OK IS JOIN SUCCESS
	127.0.0.1	127.0.0.1	IPK24-CHAT	111	Server → C | MSG FROM Server IS palop has joined default
	127.0.0.1	127.0.0.1	IPK24-CHAT	111	Server → C | MSG FROM Server IS palop has joined default
	127.0.0.1	127.0.0.1	IPK24-CHAT	111	Server → C | MSG FROM Server IS palop has joined default
	127.0.0.1	127.0.0.1	IPK24-CHAT	111	Server → C | MSG FROM Server IS palop has joined default
	127.0.0.1	127.0.0.1	IPK24-CHAT	71	C → Server | bYe
	127.0.0.1	127.0.0.1	IPK24-CHAT	105	Server → C | MSG FROM Server IS 4 has left default
	127.0.0.1	127.0.0.1	IPK24-CHAT	105	Server → C | MSG FROM Server IS 4 has left default
	127.0.0.1	127.0.0.1	IPK24-CHAT	105	Server → C | MSG FROM Server IS 4 has left default
	127.0.0.1	127.0.0.1	IPK24-CHAT	71	Server → C | BYE
	127.0.0.1	127.0.0.1	IPK24-CHAT	71	Server → C | BYE
	127.0.0.1	127.0.0.1	IPK24-CHAT	71	Server → C | BYE


## Bibliography

- [Project1] Dolejška, D. Client for a chat server using IPK24-CHAT protocol [online]. February 2024. [cited 2024-02-14]. Available at: https://git.fit.vutbr.cz/NESFIT/IPK-Projects-2024/src/branch/master/Project%201
- [RFC768] Postel, J. _User Datagram Protocol_ [online]. March 1997. [cited 2024-02-11]. DOI: 10.17487/RFC0768. Available at: https://datatracker.ietf.org/doc/html/rfc768
- [RFC9293] Eddy, W. Transmission Control Protocol (TCP) [online]. August 2022. [cited 2024-02-11]. DOI: 10.17487/RFC9293. Available at: https://datatracker.ietf.org/doc/html/rfc9293
