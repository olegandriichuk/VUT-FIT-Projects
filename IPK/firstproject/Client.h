//
// Created by oleg on 31.3.24.
//

#ifndef IPKPROJEKTOOP_CLIENT_H
#define IPKPROJEKTOOP_CLIENT_H


#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>


#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <utility>
#include <string>
#include <sstream>
#include <cctype>
#include <unistd.h>  // close

#include <sys/epoll.h>
#include <sys/socket.h>
const int stdin_fd = 0;
using namespace std;
#define MAX_EVENTS 1
#define BUFFER_SIZE 1024
/*brief
 * enuma
 * enumeration which represents states of FSM
 * */
enum STATES{
    Start,
    Auth,
    Open,
    Error,
    End
};
/** brief
 * class Client represents Client for a chat server
 */
class Client {
public:
    // Constructor: Initializes a new client with specified port, hostname, and protocol.
    // Parameters:
    // _port: The port number on which the client will attempt to connect to the server.
    // _hostname: The hostname or IP address of the server to which the client will connect.
    // _protocol: The protocol used for communication (e.g., TCP, UDP).
    Client(u_int16_t _port, const std::string& _hostname, const std::string& _protocol);

    // Destructor: Handles cleanup tasks for the client, if any are necessary.
    ~Client() {};

    // Sends a message to the server.
    // Parameters:
    // buffer: The message to be sent, contained in a character array.
    void sendClient(const char* buffer);

    // Receives a message from the server and stores it in the provided buffer.
    // Parameters:
    // buffer: A character array where the received message will be stored.
    void receiveFromServer(char* buffer);

    // Gets the current state of the client (e.g., connected, disconnected).
    // Returns: The current state of the client as an enumeration value.
    STATES getState();

    // Gets the socket file descriptor associated with the client.
    // Returns: The socket file descriptor.
    int getSockfd();

    // Gets the hostname of the server to which the client is connected or will connect.
    // Returns: The server's hostname as a string.
    string getHostname();

    // Gets the protocol used for communication with the server.
    // Returns: The communication protocol as a string.
    string getProtocol();

    // Gets the username associated with the client.
    // Returns: The client's username as a string.
    string getUsername();

    // Gets the display name associated with the client.
    // Returns: The client's display name as a string.
    string getDisplayName();

    // Gets the secret (password or token) associated with the client.
    // Returns: The client's secret as a string.
    string getSecret();



    // Gets the port number on which the client communicates with the server.
    // Returns: The communication port number.
    int getPort();

    // Initializes client resources, such as creating a socket.
    void clientCreation();

    // Connects the client to the server using previously specified hostname and port.
    void clientConnect();

    // Disconnects the client from the server.
    // Parameters:
    // epoll_fd: The file descriptor for the epoll instance, used if epoll is being utilized for asynchronous IO.
    void clientDisconnect(int epoll_fd);

private:
    // Socket file descriptor for the client's network connection.
    int sockfd;
    // Hostname or IP address of the server.
    string hostname;
    // Protocol used for communication (could indicate TCP, UDP, etc.).
    string protocol;
    // Port number for the server connection.
    u_int16_t port;
    // Current state of the client (e.g., starting, connected, disconnected).
    STATES state;
    // Username associated with the client.
    string userName;
    // Display name chosen by the user, potentially different from the username.
    string displayName;
    // Secret or password used for authentication with the server.
    string secret;
    // Server address structure used in network connections.
    struct sockaddr_in serv_addr{};
    struct hostent* host;
    socklen_t len_addr = sizeof(serv_addr);
};

#endif //IPKPROJEKTOOP_CLIENT_H
