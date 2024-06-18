#include <iostream>
using namespace std;
#include "Server.h"
#include "tcpClient.h"
#include "serverReplies.h"

#include "interrupt.h"


int main(int argc, char* argv[]) {
    int opt;
    u_int16_t port = 4567;
    u_int16_t timeout = 250;
    /*u_int8_t*/ int retransmission = 3;
    string hostname = "0.0.0.0";

    while ((opt = getopt(argc, argv, "l:p:d:r:h")) != -1) {
        switch (opt) {
            case 'l':

                hostname = optarg;

                break;
            case 'p':
                port = atoi(optarg);
                break;
            case 'd':
                timeout = atoi(optarg);
                break;
            case 'r':
                retransmission = atoi(optarg);
                break;
            case 'h': // Intentional fallthrough for help or unrecognized options
            default:
                cerr << "Usage: ./ipk24chat-server -l <hostname> -p <port> -d <timeout> -r <retransmission>\n";
                return EXIT_SUCCESS;
        }
    }

    Server Server(hostname, port);
    Server.ServerCreation();
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("epoll_create1 failed");
        return -1;
    }

    struct epoll_event ev{}, events[MAX_EVENTS];
    ev.events = EPOLLIN;

    ev.data.fd = Server.get_listenSock();


    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, Server.get_listenSock(), &ev) == -1) {
        perror("epoll_ctl: listen_sock");
        return -1;
    }
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    vector <string> words;
    vector <tcpClient*> clients(0);
   struct sigaction sigIntHandler{};
    set_cc_handler(&sigIntHandler);

    while (true) {
        int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            if (errno == EINTR) {
                if (force_quit){ // C-c handling
                    for (size_t i = 0; i < clients.size(); i++) {
                        Server.sendClient("BYE\r\n", clients[i]->getClientSock());
                        cout << "SENT " << clients[i]->getIPAddress() << ":" << clients[i]->getPort() << " | " << "BYE" << endl;
                    }

                    // After sending all messages, close all client sockets and clean up
                    for (size_t i = 0; i < clients.size(); i++) {
                        if (clients[i] != nullptr) {
                            close(clients[i]->getClientSock()); // Close the client socket
                            delete clients[i];               // Deallocate memory for client
                            clients[i] = nullptr;            // Set the pointer to nullptr to avoid dangling references
                        }
                    }

                    clients.clear(); // Clear the vector of clients to clean up

                    close(epoll_fd);           // Close the epoll file descriptor
                    close(Server.get_listenSock());    // Close the primary listen socket
                    exit(EXIT_SUCCESS);
                } else {

                    continue;
                }
            } else {
                perror("epoll_wait");
                return -1;
            }
        }

        for (int n = 0; n < nfds; ++n) {
            if (events[n].data.fd == Server.get_listenSock()) {
                // Accept new connection
                struct sockaddr_in client_addr{};
                socklen_t addrlen = sizeof(client_addr);
                int client_sock = accept(Server.get_listenSock(), (struct sockaddr *) &client_addr, &addrlen);
                // Set new socket to non-blocking
                Server.make_socket_non_blocking(client_sock);

                char ip_address[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &client_addr.sin_addr, ip_address, INET_ADDRSTRLEN);
                u_int16_t client_port = htons(client_addr.sin_port);
                auto TcpClient = new tcpClient(client_sock, ip_address, client_port);
                clients.push_back(TcpClient);
                ev.events = EPOLLIN | EPOLLET;  // Edge Triggered
                ev.data.fd = TcpClient->getClientSock();
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, TcpClient->getClientSock(), &ev);

            } else {

                int client_sock = events[n].data.fd;



                if (events[n].events & EPOLLIN) {
                    // Read data from client
                    Server.receiveFromClient(buffer, client_sock);
                    tcpClient* TcpClient_ptr;
                    for (auto client : clients) {
                        if (client->getClientSock() == client_sock) {
                            TcpClient_ptr = client;
                            break;
                        }
                        else{
                            TcpClient_ptr = nullptr;
                        }
                    }

                    if(TcpClient_ptr == nullptr){
                        break; // No client found, break the loop
                    }



                    switch (TcpClient_ptr->getState()) {

                        case Accept:
                            words = parseLine(buffer);
                            if(TcpClient_ptr->toUpperCase(words[0]) == "AUTH" && TcpClient_ptr->isValidUsername(words[1]) && TcpClient_ptr->toUpperCase(words[2]) == "AS" &&
                                    TcpClient_ptr->isValidDisplayName(words[3]) && TcpClient_ptr->toUpperCase(words[4]) == "USING" &&
                                    TcpClient_ptr->isValidSecret(words[5]) ){
                                cout << "RECV " << TcpClient_ptr->getIPAddress() << ":" << TcpClient_ptr->getPort() << " | " << "AUTH" << endl;
                                TcpClient_ptr->setDisplayName(words[3]);

                                constructPositiveAuthReply(buffer);
                                Server.sendClient(buffer, TcpClient_ptr->getClientSock());
                                cout << "SENT " << TcpClient_ptr->getIPAddress() << ":" << TcpClient_ptr->getPort() << " | " << "REPLY" << endl;
                                TcpClient_ptr->setState(Open);
                                constructJoin(buffer, TcpClient_ptr->getChannelId(), TcpClient_ptr->getDisplayName());

                                for (auto other_client : clients) {
                                    if(other_client->getState() == Open){
                                        if(other_client->getChannelId() == "default")
                                        {

                                                Server.sendClient(buffer, other_client->getClientSock());

                                        }
                                    }
                                }
                            } else{
                                cout << "RECV " << TcpClient_ptr->getIPAddress() << ":" << TcpClient_ptr->getPort() << " | " << "AUTH" << endl;
                                constructNegativeAuthReply(buffer);
                                Server.sendClient(buffer, TcpClient_ptr->getClientSock());
                                cout << "SENT " << TcpClient_ptr->getIPAddress() << ":" << TcpClient_ptr->getPort() << " | " << "REPLY" << endl;
                                TcpClient_ptr->setState(Auth);
                            }
                            break;
                        case Auth:
                            words = parseLine(buffer);
                            if(TcpClient_ptr->toUpperCase(words[0]) == "AUTH" && TcpClient_ptr->isValidUsername(words[1]) && TcpClient_ptr->toUpperCase(words[2]) == "AS" &&
                               TcpClient_ptr->isValidDisplayName(words[3]) && TcpClient_ptr->toUpperCase(words[4]) == "USING" &&
                               TcpClient_ptr->isValidSecret(words[5]) ){
                                cout << "RECV " << TcpClient_ptr->getIPAddress() << ":" << TcpClient_ptr->getPort() << " | " << "AUTH" << endl;
                                TcpClient_ptr->setDisplayName(words[3]);
                                constructPositiveAuthReply(buffer);
                                Server.sendClient(buffer, TcpClient_ptr->getClientSock());
                                cout << "SENT " << TcpClient_ptr->getIPAddress() << ":" << TcpClient_ptr->getPort() << " | " << "REPLY" << endl;
                                TcpClient_ptr->setState(Open);
                                constructJoin(buffer, TcpClient_ptr->getChannelId(), TcpClient_ptr->getDisplayName());

                                for (auto other_client : clients) {
                                    if(other_client->getState() == Open){
                                        if(other_client->getChannelId() == "default")
                                        {

                                                Server.sendClient(buffer, other_client->getClientSock());

                                        }
                                    }
                                }
                            } else{
                                constructNegativeAuthReply(buffer);
                                Server.sendClient(buffer, client_sock);
                                cout << "SENT " << TcpClient_ptr->getIPAddress() << ":" << TcpClient_ptr->getPort() << " | " << "REPLY" << endl;
                                TcpClient_ptr->setState(Auth);
                            }
                            break;
                        case Open: {
                            words = parseLine(buffer);
                            if (TcpClient_ptr->toUpperCase(words[0]) == "MSG" && TcpClient_ptr->toUpperCase(words[1]) == "FROM" &&
                                TcpClient_ptr->isValidDisplayName(words[2]) && TcpClient_ptr->toUpperCase(words[3]) == "IS") {
                                TcpClient_ptr->setDisplayName(words[2]);
                                cout << "RECV " << TcpClient_ptr->getIPAddress() << ":" << TcpClient_ptr->getPort()
                                     << " | " << "MSG" << endl;
                                constructMessage(buffer, TcpClient_ptr->getDisplayName(), words[3], words[1], words[0]);
                                for (auto other_client : clients) {
                                    if(other_client->getState() == Open){
                                        if(other_client->getChannelId() == TcpClient_ptr->getChannelId())
                                        {
                                            if (other_client->getClientSock() != TcpClient_ptr->getClientSock()) {
                                                Server.sendClient(buffer, other_client->getClientSock());
                                                cout << "SENT " << other_client->getIPAddress() << ":" << other_client->getPort()
                                                     << " | " << "MSG" << endl;
                                            }
                                        }
                                    }
                                }
                                TcpClient_ptr->setState(Open);
                            } else if (TcpClient_ptr->toUpperCase(words[0]) == "JOIN" && TcpClient_ptr->toUpperCase(words[2]) == "AS") {
                                if (TcpClient_ptr->isValidChannelId(words[1]) && TcpClient_ptr->isValidDisplayName(words[3])) {
                                    cout << "RECV " << TcpClient_ptr->getIPAddress() << ":" << TcpClient_ptr->getPort()
                                         << " | " << "JOIN" << endl;
                                    constructPositiveJoinReply(buffer);
                                    Server.sendClient(buffer, TcpClient_ptr->getClientSock());
                                    cout << "SENT " << TcpClient_ptr->getIPAddress() << ":" << TcpClient_ptr->getPort()
                                         << " | " << "REPLY" << endl;
                                    if (!TcpClient_ptr->getChannelId().empty()) {
                                        constructChannelLeaving(buffer, TcpClient_ptr->getChannelId(),
                                                                TcpClient_ptr->getDisplayName());
                                        for (auto other_client : clients) {
                                            if(other_client->getState() == Open){
                                                if(other_client->getChannelId() == TcpClient_ptr->getChannelId())
                                                {
                                                    if (other_client->getClientSock() != TcpClient_ptr->getClientSock()) {
                                                        Server.sendClient(buffer, other_client->getClientSock());
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    TcpClient_ptr->setChannelId(words[1]);
                                    constructJoin(buffer, TcpClient_ptr->getChannelId(),
                                                  TcpClient_ptr->getDisplayName());
                                    for (auto other_client : clients) {
                                        if(other_client->getState() == Open){
                                            if(other_client->getChannelId() == TcpClient_ptr->getChannelId())
                                            {

                                                    Server.sendClient(buffer, other_client->getClientSock());

                                            }
                                        }
                                    }
                                } else {
                                    cout << "RECV " << TcpClient_ptr->getIPAddress() << ":" << TcpClient_ptr->getPort()
                                         << " | " << "JOIN" << endl;
                                    constructNegativeJoinReply(buffer);
                                    Server.sendClient(buffer, TcpClient_ptr->getClientSock());
                                    cout << "SENT " << TcpClient_ptr->getIPAddress() << ":" << TcpClient_ptr->getPort()
                                         << " | " << "REPLY" << endl;
                                }

                                TcpClient_ptr->setState(Open);
                            } else if (TcpClient_ptr->toUpperCase(words[0]) == "ERR" && TcpClient_ptr->toUpperCase(words[1]) == "FROM" &&
                                    TcpClient_ptr->isValidDisplayName(words[2]) && TcpClient_ptr->toUpperCase(words[3]) == "IS") {
                                cout << "RECV " << TcpClient_ptr->getIPAddress() << ":" << TcpClient_ptr->getPort()
                                     << " | " << "ERR" << endl;
                                Server.sendClient("BYE\r\n", TcpClient_ptr->getClientSock());
                                cout << "SENT " << TcpClient_ptr->getIPAddress() << ":" << TcpClient_ptr->getPort()
                                     << " | " << "BYE" << endl;
                                TcpClient_ptr->setState(End);
                                Server.leaveServer(buffer, TcpClient_ptr, clients);
                            } else if (TcpClient_ptr->toUpperCase(words[0]) == "BYE") {
                                cout << "RECV " << TcpClient_ptr->getIPAddress() << ":" << TcpClient_ptr->getPort()
                                     << " | " << "BYE" << endl;
                                TcpClient_ptr->setState(End);
                                Server.leaveServer(buffer, TcpClient_ptr, clients);
                            } else {
                                TcpClient_ptr->setState(Error);
                                constructError(buffer);
                                Server.sendClient(buffer, TcpClient_ptr->getClientSock());
                                cout << "SENT " << TcpClient_ptr->getIPAddress() << ":" << TcpClient_ptr->getPort()
                                     << " | " << "ERR" << endl;
                                Server.sendClient("BYE\r\n", TcpClient_ptr->getClientSock());
                                cout << "SENT " << TcpClient_ptr->getIPAddress() << ":" << TcpClient_ptr->getPort()
                                     << " | " << "BYE" << endl;
                                TcpClient_ptr->setState(End);
                                Server.leaveServer(buffer, TcpClient_ptr, clients);
                            }
                            break;
                        }

                    }
                }
            }
        }
    }

    return 0;
}
