//
// Created by oleg on 31.3.24.
//

#include "constructMessages.h"
void constructPositiveReply(char* buffer){
    // Convert the C-style string buffer to a C++ string for easier manipulation
    std::string message = buffer;

    // Create a new string by concatenating "Success:" with the part of the message after the 11th character
    std::string res ="Success:" + message.substr(11) + '\n';

    // Copy the resulting string back into the provided buffer.
    // Use std::strncpy to avoid buffer overflow, copying at most BUFFER_SIZE - 1 characters to ensure space for the null terminator
    std::strncpy(buffer, res.c_str(), BUFFER_SIZE - 1);

    // Manually null-terminate the buffer at its last position to ensure the string is properly terminated
    buffer[BUFFER_SIZE  - 1] = '\0';

    // Output the modified buffer to the standard error stream for debugging or logging purposes
    std::cerr << buffer;
}
void constructNegativeReply(char* buffer){
    string message = buffer;
    string res ="Failure:" + message.substr(12) + '\n';
    std::strncpy(buffer, res.c_str(), BUFFER_SIZE - 1);

    // Manually null-terminate the buffer
    buffer[BUFFER_SIZE  - 1] = '\0';
    cerr << buffer;
}
void convertErrorMessage(char* buffer) {
    const char* isKeyword = " IS ";
    char* isLocation = strstr(buffer, isKeyword);

    if (isLocation != nullptr) {
        // Shift everything after " IS " one position to the left to make room for ": "
        size_t shiftLength = strlen(isLocation + strlen(isKeyword));
        memmove(isLocation + 2, isLocation + strlen(isKeyword), shiftLength + 1); // +1 for null terminator

        // Replace " IS " with ": "
        isLocation[0] = ':';
        isLocation[1] = ' ';

        // Find the position of "\r\n" and replace it with "\n"
        char* endOfMessage = strstr(buffer, "\r\n");
        if (endOfMessage != nullptr) {
            endOfMessage[0] = '\n'; // Replace '\r' with '\n'
            endOfMessage[1] = '\0'; // Null-terminate the string
        }
    }
    cerr << buffer << endl;
}
void convertMessage(char* buffer) {
    // Step 1: Remove "MSG FROM " prefix by finding its position and shifting the string left.
    const char* prefix = "MSG FROM ";
    if (strncmp(buffer, prefix, strlen(prefix)) == 0) {
        char* startAfterPrefix = buffer + strlen(prefix);
        memmove(buffer, startAfterPrefix, strlen(startAfterPrefix) + 1); // +1 for null terminator
    }

    // Step 2: Replace the first occurrence of " IS " with ": ".
    char* isLocation = strstr(buffer, " IS ");
    if (isLocation != nullptr) {
        *isLocation = ':'; // Replace 'I' of " IS " with ':'
        *(isLocation + 1) = ' ';
        memmove(isLocation + 2, isLocation + 4, strlen(isLocation + 4) + 1); // +1 for null terminator
    }

    // Step 3: Replace "\r\n" with "\n" at the end of the string, if present.
    size_t len = strlen(buffer);
    if (len >= 2 && buffer[len - 2] == '\r' && buffer[len - 1] == '\n') {
        buffer[len - 2] = '\n';
        buffer[len - 1] = '\0'; // Correctly null-terminate the string
    }

    std::cout << buffer << std::endl;
}
void constructAuth(char* buffer, const std::string& Username, const std::string& Displayname, const std::string& secret){
    // Formats the authentication message using the provided username, display name, and secret
    std::string res = "AUTH " + Username + " AS "+ Displayname + " USING " + secret + '\r' + '\n';

    // Safely copies the formatted string into the buffer, ensuring no overflow
    strncpy(buffer, res.c_str(), BUFFER_SIZE - 1);

    // Ensures the buffer is null-terminated to prevent string manipulation errors
    buffer[BUFFER_SIZE - 1] = '\0';
}

// Constructs a join channel message and stores it in the provided buffer.
void constructJoin(char* buffer, const std::string& channelId, const std::string& Displayname){
    // Formats the join message using the provided channel ID and display name
    std::string res = "JOIN " + channelId + " AS "+ Displayname + '\r' + '\n';

    // Copies the message into the buffer, ensuring the buffer size is not exceeded
    strncpy(buffer, res.c_str(), BUFFER_SIZE - 1);

    // Null-terminates the buffer to correctly end the string
    buffer[BUFFER_SIZE - 1] = '\0';
}

// Constructs a message from the user and stores it in the provided buffer.
void constructMessage(char* buffer, const std::string& Displayname){
    // Initially, takes the existing content of the buffer as the message content
    std::string messageContent = buffer;

    // Formats the message prefix and appends the original message content
    std::string res = "MSG FROM " + Displayname + " IS "+ messageContent + '\r' + '\n';

    // Copies the full message back into the buffer, ensuring it fits
    strncpy(buffer, res.c_str(), BUFFER_SIZE - 1);

    // Ensures the message is properly null-terminated
    buffer[BUFFER_SIZE - 1] = '\0';
}

void constructError(char* buffer, const std::string& Displayname){
    // Initially, takes the existing content of the buffer as the message content
    std::string messageContent = buffer;

    // Formats the message prefix and appends the original message content
    std::string res = "ERROR FROM " + Displayname + " IS "+ messageContent + '\r' + '\n';

    // Copies the full message back into the buffer, ensuring it fits
    strncpy(buffer, res.c_str(), BUFFER_SIZE - 1);

    // Ensures the message is properly null-terminated
    buffer[BUFFER_SIZE - 1] = '\0';
}

// Constructs an error message from the client and stores it in the provided buffer.
void constructErrorFromClient(char* buffer, const std::string& Displayname){
    // Formats the error message using the provided display name
    std::string res = "ERROR FROM " + Displayname + " IS "+ "UNDEFINED MESSAGE FROM SERVER" + '\r' + '\n';

    // Safely copies the error message into the buffer
    strncpy(buffer, res.c_str(), BUFFER_SIZE - 1);

    // Null-terminates the buffer to ensure proper string termination
    buffer[BUFFER_SIZE - 1] = '\0';

    // Outputs the error message to the standard error stream for logging or debugging
    std::cerr << buffer;
}

string extractAndCheckMessage(const std::string& message) {
    std::istringstream iss(message);
    std::string word1;
    word1.clear();
    iss >> word1;
    return word1;
}

string extractSecondWord(const std::string& message){
    std::istringstream iss(message);
    std::string word1, word2;
    word1.clear();
    word2.clear();
    // read first two words from a line
    iss >> word1 >> word2;

    return word2;
}

string extractThirdWord(const std::string& message){
    std::istringstream iss(message);
    std::string word1, word2, word3;
    word1.clear();
    word2.clear();
    word3.clear();
    // read first two words from a line
    iss >> word1 >> word2 >> word3;

    return word3;
}

string extractFourthWord(const std::string& message){
    std::istringstream iss(message);
    std::string word1, word2, word3, word4;
    word1.clear();
    word2.clear();
    word3.clear();
    word4.clear();
    // read first two words from a line
    iss >> word1 >> word2 >> word3 >> word4;

    return word4;
}