//
// Created by oleg on 31.3.24.
//

#include "Client.h"

Client::Client(u_int16_t _port, const std::string& _hostname, const string& _protocol):
        port(_port),
        hostname(_hostname),
        protocol(_protocol)
{}

void Client::sendClient(const char *buffer) {

        ssize_t bytestx = send(this->sockfd, buffer, strlen(buffer), 0); // Send data including newline

        if (bytestx < 0) {
            cerr << "ERROR in send" << endl;
            exit(EXIT_FAILURE);
        }
}

void Client::receiveFromServer(char *buffer) {
    char *tmp;
    ssize_t bytesrc = recv(sockfd, buffer, BUFFER_SIZE - 1, 0);
    if (bytesrc < 0) {
        cerr<< "ERROR in recvfrom" << endl;
        exit(EXIT_FAILURE);
    }
    tmp = strchr(buffer, '\n');  // Replacing newline cahracter, otherwise strcmp would not work
    if (tmp != nullptr) {
        *tmp = '\0';
    }
}

string Client::getHostname() {
    return this->hostname;
}

STATES Client::getState() {
    return this->state;
}

string Client::getProtocol() {
    return this->protocol;
}

int Client::getPort() {
    return this->port;
}

void Client::clientCreation() {
    // Sets the protocol to TCP explicitly.
    protocol = "tcp";
    this->state = Start;

    host = gethostbyname(this->hostname.c_str());
    if (host == NULL) {
        cerr << "Failed to resolve hostname!" << endl;
        return;
    }
    // Creates a new socket for IPv4 connections using TCP (SOCK_STREAM).
    // AF_INET specifies the address family for IPv4.
    // SOCK_STREAM specifies a reliable, connection-based byte stream (TCP in this case).
    // The third argument is the protocol, where 0 specifies that the OS should choose the protocol (TCP for SOCK_STREAM).
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Checks if socket creation was successful.
    // A return value of 0 or less indicates an error.
    if (sockfd < 0) {
        // Outputs an error message to standard error if socket creation fails.
        std::cerr << "ERROR: socket" << std::endl;
        // Exits the program indicating failure, to prevent further erroneous operations.
        //exit(EXIT_FAILURE);
        return;
    }

    // Initializes the server address structure to zeros.
    // This is a common practice to clear out any existing garbage values.
    memset(&serv_addr, 0, this->len_addr);

    // Sets the address family to IPv4.
    serv_addr.sin_family = AF_INET;

    // Converts the port number from host byte order to network byte order (big-endian)
    // and assigns it to the server address structure.
    memcpy((&(serv_addr).sin_addr.s_addr), host->h_addr, host->h_length);
    serv_addr.sin_port = htons(this->port);

    // Checks if a hostname has been provided.

}

void Client::clientConnect() {
    // creating connection for tcp
    if (::connect(this->sockfd, (struct sockaddr *)&serv_addr, len_addr) == -1) {
        perror("Error connecting to server");
        close(sockfd); // Ensure we close the socket on error
        throw std::runtime_error("Connection to server failed");
    }
   // std::cout << "You are connected to the server." << std::endl;
}

void Client::clientDisconnect(int epoll_fd) {
    // Closes the epoll file descriptor.
    // epoll_fd is assumed to be an epoll instance file descriptor used for monitoring multiple file descriptors.
    close(epoll_fd);

    // Shuts down the socket connection in both directions (read and write).
    // SHUT_RDWR is used to disable further receptions and transmissions.

    shutdown(this->sockfd, SHUT_RDWR);

    // Closes the socket file descriptor.
    // This releases the resources allocated to this socket by the operating system.
    // After this call, this->sockfd is no longer valid and should not be used.
    close(sockfd);
}

string Client::getUsername() {
    return userName;
}

string Client::getSecret() {
    return secret;
}

string Client::getDisplayName() {
    return displayName;
}

int Client::getSockfd() {
    return sockfd;
}
