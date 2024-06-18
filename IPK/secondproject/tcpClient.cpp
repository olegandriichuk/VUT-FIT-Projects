//
// Created by oleg on 16.4.24.
//

#include "tcpClient.h"

#include <utility>

int tcpClient::getClientSock() {
    return m_clientSock;
}

std::string tcpClient::getDisplayName() {
    return m_displayName;
}

std::string tcpClient::getChannelId() {
    return m_channelId;
}


STATES tcpClient::getState() {
    return m_state;
}

void tcpClient::setDisplayName(std::string _displayName) {
    m_displayName = std::move(_displayName);
}

void tcpClient::setChannelId(std::string _channelId) {
    m_channelId = std::move(_channelId);
}

void tcpClient::setState(STATES _state) {
    m_state = _state;
}


std::string tcpClient::getIPAddress() {
    return m_IPAddress;
}

u_int16_t tcpClient::getPort() {
    return m_port;
}

tcpClient::tcpClient(int _clientSock, std::string IPAddress, u_int16_t _port) :
        m_clientSock(_clientSock),
        m_IPAddress(std::move(IPAddress)),
        m_port(_port)
{};

bool tcpClient::isValidChannelId(const string& channelId) {

    size_t len = channelId.length();
    if (len > 20 || len == 0) return false;
    for (size_t i = 0; i < len; ++i) {
        if (!isalnum(channelId[i]) && channelId[i] != '-') return false;
    }
    return true;
}
bool tcpClient::isValidUsername(const string& username) {

    size_t len = username.length();
    if (len > 20 || len == 0) return false;
    for (size_t i = 0; i < len; ++i) {
        if (!isalnum(username[i]) && username[i] != '-') return false;
    }
    return true;
}

bool tcpClient::isValidDisplayName(const  string&  displayname) {

    size_t len = displayname.length();
    if (len > 20 || len == 0) return false;
    for (size_t i = 0; i < len; ++i) {
        if (!isprint(displayname[i])) return false;
    }
    return true;
}

bool tcpClient::isValidSecret(const  string&  secret) {

    size_t len = secret.length();
    if (len > 128 || len == 0) return false;
    for (size_t i = 0; i < len; ++i) {
        if (!isalnum(secret[i]) && secret[i] != '-') return false;
    }
    return true;
}

std::string tcpClient::toUpperCase(const std::string& input) {
    std::string result = input;
    transform(result.begin(), result.end(), result.begin(),
              [](unsigned char c) { return std::toupper(c); });
    return result;
}

