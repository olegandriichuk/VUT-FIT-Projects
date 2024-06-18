//
// Created by oleg on 15.4.24.
//

#ifndef IPKSERVEROOP_SERVER_H
#define IPKSERVEROOP_SERVER_H
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
#include <fcntl.h>
#include <map>
#include "tcpClient.h"
#include <csignal>
#include <atomic>
#define MAX_EVENTS 10
#define BUFFER_SIZE 1024

class Server {
public:
    Server(const std::string& _hostname, u_int16_t port);
    ~Server()= default;
    void ServerCreation();

    int get_listenSock();
    int make_socket_non_blocking(int _clientSock);

    void sendClient(const char *buffer, int clientSocket);


    void receiveFromClient(char *buffer, int clientSocket);

    void removeClient(std::vector<tcpClient*>& clients, tcpClient* clientToRemove);



    void leaveServer(char* buffer, tcpClient* TcpClient_ptr, std::vector<tcpClient*>& clients);


private:
    int m_listenSock;
    std::string m_hostname;
    u_int16_t m_port;
    u_int16_t  m_timeout;
    int m_retransmission;


};


#endif //IPKSERVEROOP_SERVER_H
