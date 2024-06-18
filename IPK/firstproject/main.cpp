#include <iostream>
#include "Client.h"
#include "dataValidation.h"
#include "constructMessages.h"
#include "Connection.h"

int main(int argc, char* argv[]){
    //parce_arguments(argc, argv);
    int opt;
    u_int16_t port = 4567;
    u_int16_t  timeout = 250;
    u_int8_t retransmission = 3;
    string hostname;
    string protocol;
    bool hostname_provided = false;  // Flag to check if hostname is provided
    bool protocol_provided = false;  // Flag to check if protocol is provided


    while((opt = getopt(argc, argv, "t:s:p:d:r:"))!= -1){
        switch (opt) {
            case 't':
                protocol = optarg;
                protocol_provided = true;
                break;
            case 's':
                if(optarg[0] == '-') { // Check if optarg is another option
                    cerr << "Error: Hostname not specified after -s\n";
                    return EXIT_FAILURE;
                }
                hostname = optarg;
                hostname_provided = true;
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
                cerr << "Usage: ./ipk24-chat -t <protocol> -s <hostname>\n";
                return EXIT_SUCCESS;
        }
    }

    // After parsing, check if the required arguments are provided
    if (!hostname_provided) {
        cerr << "Error: Hostname not specified!\nUsage: ./ipk24-chat -t <protocol> -s <hostname>\n";
        return EXIT_FAILURE;
    }

    if (port  == 0) {
        cerr << "Error: Port not specified!\nUsage: ./ipk24-chat -t <protocol> -s <hostname>\n";
        return EXIT_FAILURE;
    }

    if (!protocol_provided) {
        cerr << "Error: Protocol not specified!\nUsage: ./ipk24-chat -t <protocol> -s <hostname>\n";
        return EXIT_FAILURE;
    }

    if (protocol != "tcp" && protocol != "udp") {
        cerr << "Error: Invalid protocol specified! Use 'tcp' or 'udp'.\n";
        return EXIT_FAILURE;
    }
    // Creates an instance of the Client class named tcpProtocolClient.
    Client tcpProtocolClient(port, hostname, protocol);

    // Checks if the specified protocol for the tcpProtocolClient instance is "tcp".
    // This condition ensures that the clientCreation method, which sets up the client for TCP communication,
    // is only called if the intended protocol is indeed TCP.
    if(protocol == "tcp"){
        tcpProtocolClient.clientCreation();
    }
    // Initiates a connection to the server using the tcpProtocolClient's details.
    tcpProtocolClient.clientConnect();

// Creates an epoll instance to monitor file descriptors (FDs) for I/O events.
// The '0' argument specifies no special flags to epoll_create1().
    int epoll_fd = epoll_create1(0);
// Checks if the epoll instance was successfully created.
    if(epoll_fd == -1){
        // If epoll_create1 failed, outputs an error message and closes the socket.
        cerr << "ERR: Could not create epoll instance" << std::endl;
        // Uses the getSockfd() method to retrieve the socket file descriptor and close it.
        close(tcpProtocolClient.getSockfd());
        // Exits the program indicating a failure to create the epoll instance.
        return EXIT_FAILURE;
    }

// Defines epoll_event structures to configure what events to monitor for the socket and stdin.
    struct epoll_event sock_event;
    struct epoll_event stdin_event;
    int rval; // Variable to store the return value of epoll_ctl calls.

// Configures the sock_event and stdin_event structures to monitor for EPOLLIN (readable data available).
    sock_event.events = EPOLLIN;
    stdin_event.events = EPOLLIN;

// Associates the socket FD and stdin FD with the respective epoll_event structures.
    sock_event.data.fd = tcpProtocolClient.getSockfd(); // Socket file descriptor.
    stdin_event.data.fd = stdin_fd; // Standard input file descriptor (presumably defined elsewhere).

// Adds the socket FD to the epoll instance for monitoring.
    rval = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, tcpProtocolClient.getSockfd(), &sock_event);
// Checks if the operation was successful.
    if(rval != 0){
        // If unsuccessful, exits the program indicating failure.
        exit(EXIT_FAILURE);
    }

// Adds the stdin FD to the epoll instance for monitoring.
    rval = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, stdin_fd, &stdin_event);
// Checks if the operation was successful.
    if(rval != 0){
        // If unsuccessful, exits the program indicating failure.
        exit(EXIT_FAILURE);
    }

// Allocates memory for storing epoll events, with MAX_EVENTS presumably defined elsewhere.
    struct epoll_event events[MAX_EVENTS];

// Retrieves the initial state of the tcpProtocolClient.
    STATES state = tcpProtocolClient.getState();
// A flag to control the exit condition of the loop that may follow for handling events.
    bool out_flag = false;
// Buffer for reading input or receiving messages.
    char buffer[BUFFER_SIZE];
    //strings for checking grammar from server
    string commandType, secondWord, thirdWord, fourthWord;
    std::string username = tcpProtocolClient.getUsername();
    string displayname = tcpProtocolClient.getDisplayName();
    string secret = tcpProtocolClient.getSecret();
    string channelId, messageContent;
    // string to send Bye
    string sendBye = "BYE\r\n";
    // counter for sending bye in auth
    int counterForBye = 0;
    bool validation;
    while (!out_flag){

        // IN This while loop we are sending and receiving messages from server and going through final state machine
        // state error:
        // send bye and ends and goes to the state end
        if(state == Error){
            bzero(buffer, BUFFER_SIZE);
            strcpy(buffer, sendBye.c_str());
            tcpProtocolClient.sendClient(buffer);

            state = End;
        }
        // state end:
        // end successfully a program
        if(state == End){
            out_flag = false;
            tcpProtocolClient.clientDisconnect(epoll_fd);
            exit(EXIT_SUCCESS);
        }
        // Waits indefinitely (-1 timeout) for events on file descriptors added to the epoll instance (epoll_fd).
        // The function fills the 'events' array with up to MAX_EVENTS occurrences of epoll_event structures,
        // which describe the events that occurred.
        // The function returns the number of file descriptors for which events occurred.
        int even_count = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        // Checks if the first event in the array corresponds to the standard input file descriptor (stdin_fd).
        if(events[0].data.fd == stdin_fd){
            switch (state) {
                // state start:
                // doing authentication
                case Start:
                    // function is doing all the things for authentication
                    processAuthentication( tcpProtocolClient, buffer, commandType, username, displayname, secret, counterForBye);
                    // sends the result of Authentication to server
                    if (commandType != "/help"){
                        tcpProtocolClient.sendClient(buffer);
                    }
                    state = Auth;
                    break;

                    // state Auth:
                    // doing authentication if the reply from server was negative, otherwise we will skip this state
                case Auth:
                    processAuthentication(tcpProtocolClient,buffer, commandType, username, displayname, secret, counterForBye);
                    // if the message was BYE we go to End  state
                    if(commandType == "BYE"){
                        strcpy(buffer, sendBye.c_str());
                        state = End;
                    }
                    // sends repeatedly the result of Authentication to server
                    if (commandType != "/help"){
                        tcpProtocolClient.sendClient(buffer);
                    }
                    break;
                    // state Open:
                    // send message to servers and join channels
                case Open:

                    // validation returns false if we want to rename displayId or call /help
                    validation = joinChannelProcess(tcpProtocolClient,buffer, commandType, channelId, displayname);
                    // checks what the message we sent
                    commandType = extractAndCheckMessage(buffer);
                    // /help and /rename - stay in state Open
                     if(commandType == "/help" || commandType == "/rename" || !validation){
                        state = Open;
                        // JOIN and MSG - stay in open
                    }else if((commandType == "JOIN" || commandType  == "MSG") && validation){
                        tcpProtocolClient.sendClient(buffer);
                        state = Open;
                     }
                    break;
            }
        }else{
            // receive messages from server
            tcpProtocolClient.receiveFromServer(buffer);
            switch (state) {
                // state Auth:
                // receiving replies to our authentication
                case Auth:
                    // extract words from server reply
                    commandType = extractAndCheckMessage(buffer);
                    secondWord = extractSecondWord(buffer);
                    thirdWord = extractThirdWord(buffer);
                    fourthWord = extractFourthWord(buffer);
                    // positive or negative reply
                    if (commandType == "REPLY" && thirdWord == "IS" && (secondWord == "OK" || secondWord == "NOK")) {
                        if (secondWord == "OK") {
                            constructPositiveReply(buffer);
                            state = Open;
                        } else if (secondWord == "NOK") {
                            constructNegativeReply(buffer);
                            state = Auth;
                        }
                        // server sends an error -> state End and sending automatically bye
                    }else if(commandType == "ERR" && secondWord == "FROM" && fourthWord == "IS") {
                        bzero(buffer, BUFFER_SIZE);
                        strcpy(buffer, sendBye.c_str());
                        tcpProtocolClient.sendClient(buffer);
                        state = End;
                        // if message is not our type send error and bye
                    } else {
                        bzero(buffer, BUFFER_SIZE);
                        constructErrorFromClient(buffer,displayname);
                        tcpProtocolClient.sendClient(buffer);
                        strcpy(buffer, sendBye.c_str());
                        tcpProtocolClient.sendClient(buffer);
                        tcpProtocolClient.clientDisconnect(epoll_fd);
                        exit(EXIT_SUCCESS);
                    }
                    break;
                    // case Open:
                    // receiving messages and replies from server
                case Open:
                    // extract messages from server and check if they are correct
                    commandType = extractAndCheckMessage(buffer);
                    secondWord = extractSecondWord(buffer);
                    thirdWord = extractThirdWord(buffer);
                    fourthWord = extractFourthWord(buffer);
                    if(commandType == "REPLY" && thirdWord == "IS" && (secondWord == "OK" || secondWord == "NOK")){
                        if(secondWord == "OK") {
                            constructPositiveReply(buffer);
                            state = Open;
                        }else if(commandType == "NOK"){
                            constructNegativeReply(buffer);
                            state = Open;
                        }
                    }else if((commandType == "MSG" || commandType == "ERR") && secondWord == "FROM" && fourthWord == "IS"){
                        if(commandType == "MSG") {
                            convertMessage(buffer);
                            state = Open;
                        }else if(commandType == "ERR"){
                            convertErrorMessage(buffer);
                            bzero(buffer, BUFFER_SIZE);
                            strcpy(buffer, sendBye.c_str());
                            tcpProtocolClient.sendClient(buffer);
                            state =  End;
                        }
                    }else if(commandType == "BYE"){
                        state = End;
                        // if the message is not our type -> state error and send BYE
                    }else{
                        cout<< buffer << endl;
                        bzero(buffer, BUFFER_SIZE);
                        constructErrorFromClient(buffer,displayname);
                        tcpProtocolClient.sendClient(buffer);
                        state = Error;
                    }
                    break;
            }
        }
    }
    return 0;
}