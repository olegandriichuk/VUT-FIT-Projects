//
// Created by oleg on 16.4.24.
//

#include "serverReplies.h"
#include "tcpClient.h"


void constructPositiveAuthReply(char* buffer){
    // Convert the C-style string buffer to a C++ string for easier manipulation


    // Create a new string by concatenating "Success:" with the part of the message after the 11th character
    std::string res = "REPLY OK IS AUTH SUCCESS\r\n" ;

    // Copy the resulting string back into the provided buffer.
    // Use std::strncpy to avoid buffer overflow, copying at most BUFFER_SIZE - 1 characters to ensure space for the null terminator
    std::strncpy(buffer, res.c_str(), BUFFER_SIZE - 1);

    // Manually null-terminate the buffer at its last position to ensure the string is properly terminated
    buffer[BUFFER_SIZE  - 1] = '\0';



}
void constructNegativeAuthReply(char* buffer){
    string res = "REPLY NOK IS AUTH UNSUCCESS\r\n";
    std::strncpy(buffer, res.c_str(), BUFFER_SIZE - 1);

    // Manually null-terminate the buffer
    buffer[BUFFER_SIZE  - 1] = '\0';

}

void constructPositiveJoinReply(char* buffer){
    std::string res = "REPLY OK IS JOIN SUCCESS\r\n" ;
    std::strncpy(buffer, res.c_str(), BUFFER_SIZE - 1);
    buffer[BUFFER_SIZE  - 1] = '\0';
}
void constructNegativeJoinReply(char* buffer){
    string res = "REPLY NOK IS JOIN UNSUCCESS\r\n";
    std::strncpy(buffer, res.c_str(), BUFFER_SIZE - 1);
    buffer[BUFFER_SIZE  - 1] = '\0';
}

void constructError(char* buffer){
    string res = "ERROR FROM Server IS ERROR Massage from client\r\n";
    std::strncpy(buffer, res.c_str(), BUFFER_SIZE - 1);
    buffer[BUFFER_SIZE  - 1] = '\0';
}


void constructMessage(char* buffer, const std::string& displayName, const std::string& isWord, const std::string& fromWord, const std::string& msgWord) {
    std::string content(buffer);
    std::string res;

    size_t fromIndex = content.find(fromWord);
    size_t isIndex = content.find(isWord);

    if (fromIndex != std::string::npos && isIndex != std::string::npos && isIndex > fromIndex) {

        std::string beforeFrom = content.substr(0, fromIndex);
        std::string afterIs = content.substr(isIndex + 2);


        res = beforeFrom + fromWord +" " + displayName + " " + isWord + afterIs;
    } else {

        res = msgWord + " " + fromWord + " " + displayName + " " + isWord + " " + content ;
    }
    strncpy(buffer, res.c_str(), BUFFER_SIZE - 1);
    buffer[BUFFER_SIZE - 1] = '\0';
}

std::vector<std::string> parseLine(const char* buffer) {
    std::string line(buffer);  // Перетворюємо char* у std::string
    std::stringstream ss(line);
    std::string word;
    std::vector<std::string> words;

    while (ss >> word) {
        words.push_back(word);
    }

    return words;
}


void constructJoin(char* buffer, const std::string& channelId, const std::string& Displayname){
    // Formats the join message using the provided channel ID and display name
    std::string res = "MSG FROM Server IS " + Displayname + " has joined "+ channelId + '\r' + '\n';
    strncpy(buffer, res.c_str(), BUFFER_SIZE - 1);
    buffer[BUFFER_SIZE - 1] = '\0';
}

void constructChannelLeaving(char* buffer, const std::string& channelId, const std::string& Displayname){
    std::string res = "MSG FROM Server IS " + Displayname + " has left "+ channelId + '\r' + '\n';
    strncpy(buffer, res.c_str(), BUFFER_SIZE - 1);
    buffer[BUFFER_SIZE - 1] = '\0';
}


