//
// Created by oleg on 31.3.24.
//

#include "dataValidation.h"
bool isValidMessageContent(const  char*  messageContent) {

    size_t len = strlen(messageContent);
    if (len > 1400 || len == 0) return false;
    for (size_t i = 0; i < len; ++i) {
        if (!isprint(messageContent[i]) && (!isspace(messageContent[i]))) return false;
    }
    return true;
}
bool isValidChannelId(const string& channelId) {

    size_t len = channelId.length();
    if (len > 20 || len == 0) return false;
    for (size_t i = 0; i < len; ++i) {
        if (!isalnum(channelId[i]) && channelId[i] != '-') return false;
    }
    return true;
}
bool isValidUsername(const string& username) {

    size_t len = username.length();
    if (len > 20 || len == 0) return false;
    for (size_t i = 0; i < len; ++i) {
        if (!isalnum(username[i]) && username[i] != '-') return false;
    }
    return true;
}

bool isValidDisplayName(const  string&  displayname) {

    size_t len = displayname.length();
    if (len > 20 || len == 0) return false;
    for (size_t i = 0; i < len; ++i) {
        if (!isprint(displayname[i])) return false;
    }
    return true;
}

bool isValidSecret(const  string&  secret) {

    size_t len = secret.length();
    if (len > 128 || len == 0) return false;
    for (size_t i = 0; i < len; ++i) {
        if (!isalnum(secret[i]) && secret[i] != '-') return false;
    }
    return true;
}