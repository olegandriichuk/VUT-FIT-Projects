//
// Created by oleg on 15.4.24.
//

#include "Server.h"
#include "serverReplies.h"
Server::Server(const std::string& _hostname, u_int16_t _port):
m_hostname(_hostname),
m_port(_port)
{};
// Creates the server socket, sets options, binds it, and starts listening.
void Server::ServerCreation()
{
    m_listenSock = socket(AF_INET, SOCK_STREAM, 0);

    if (m_listenSock == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    int enable = 1;
    setsockopt(m_listenSock, SOL_SOCKET, SO_REUSEADDR,
               &enable, sizeof(enable));

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(m_port);

    if (bind(m_listenSock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(m_listenSock);
        exit(EXIT_FAILURE);
    }
    if (listen(m_listenSock, 10) < 0) {
        perror("listen");
        close(m_listenSock);
        exit(EXIT_FAILURE);
    }

}


int Server::get_listenSock() {
    return m_listenSock;
}
// Sets a given client socket to non-blocking mode.
int Server::make_socket_non_blocking(int _clientSock) {
    int flags, s;

    flags = fcntl(_clientSock, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl");
        return -1;
    }

    flags |= O_NONBLOCK; // Set non-blocking flag
    s = fcntl(_clientSock, F_SETFL, flags);
    if (s == -1) {
        perror("fcntl");
        return -1;
    }

    return 0;
}

void Server::sendClient(const char *buffer, int clientSocket) {

    ssize_t bytestx = send(clientSocket, buffer, strlen(buffer), 0); // Send data including newline

    if (bytestx < 0) {
        std::cerr << "ERROR in send" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Server::receiveFromClient(char *buffer, int clientSocket) {
    memset(buffer, 0, BUFFER_SIZE);
    ssize_t bytesrc = recv(clientSocket, buffer, BUFFER_SIZE, 0);
    if (bytesrc < 0) {
        std::cerr<< "ERROR in recvfrom" << std::endl;
        exit(EXIT_FAILURE);
    }
}
// Removes a client from the list of clients and deallocates its resources.
void  Server::removeClient(std::vector<tcpClient*>& clients, tcpClient* clientToRemove) {
    auto found_client = std::find(clients.begin(), clients.end(), clientToRemove);
    if (found_client != clients.end()) {
        delete *found_client; // Delete client object
        *found_client = nullptr;
        clients.erase(found_client);  // Remove from list
    } else {
        std::cout << "Client not found." << std::endl;
    }
}

// Handles client leaving the server, notifies other clients, and cleans up resources.
void Server::leaveServer(char* buffer, tcpClient* TcpClient_ptr, std::vector<tcpClient*>& clients){
    constructChannelLeaving(buffer, TcpClient_ptr->getChannelId(),
                            TcpClient_ptr->getDisplayName());
    for (auto other_client : clients) {
        if(other_client->getState() == Open){
            if(other_client->getChannelId() == TcpClient_ptr->getChannelId())
            {
                if (other_client->getClientSock() != TcpClient_ptr->getClientSock()) {
                    sendClient(buffer, other_client->getClientSock());
                }
            }
        }
    }
    close(TcpClient_ptr->getClientSock()); // Close the client's socket
    removeClient(clients, TcpClient_ptr); // Remove client from the list
}

