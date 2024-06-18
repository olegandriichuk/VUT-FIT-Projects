//
// Created by oleg on 16.4.24.
//

#ifndef IPKSERVEROOP_TCPCLIENT_H
#define IPKSERVEROOP_TCPCLIENT_H
//#include "Server.h"
using namespace std;
#include "string"
#include <cstring>
#include <algorithm>
#include <cctype>
enum STATES{
    Accept,
    Auth,
    Open,
    Error,
    End
};
class tcpClient {
public:
    tcpClient(int _clientSock, std::string IPAddress, u_int16_t _port);
    ~tcpClient()
    {};
    int getClientSock();

    std::string getDisplayName();

    std::string getChannelId();


    std::string getIPAddress();
    u_int16_t getPort();

    STATES getState();


    void setDisplayName(std::string _displayName);

    void setChannelId(std::string _channelId);

    void setState(STATES _state);



    bool isValidChannelId(const string& channelId);

/** brief
 * check if username is valid
 * @param username - clients username
 * @return true if username is valid
 */
    bool isValidUsername(const string& username);

/** brief
 * check if displayname is valid
 * @param displayname - clients displayname
 * @return true if displayname is valid
 */
    bool isValidDisplayName(const  string&  displayname);

/** brief
 * check if secret is valid
 * @param secret - clients secret
 * @return true if secret is valid
 */
    bool isValidSecret(const  string&  secret);

    std::string toUpperCase(const std::string& input);



private:
    int m_clientSock;
    STATES m_state {Accept};
    std::string m_displayName;
    std::string m_channelId{"default"};
    std::string m_Username;
    std:: string m_IPAddress;
    u_int16_t m_port;

};


#endif //IPKSERVEROOP_TCPCLIENT_H
