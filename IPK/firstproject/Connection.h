//
// Created by oleg on 31.3.24.
//

#ifndef IPKPROJEKTOOP_CONNECTION_H
#define IPKPROJEKTOOP_CONNECTION_H
#include "Client.h"
#include "dataValidation.h"
#include "constructMessages.h"
/** brief
 * provides authentication for client
 * @param buffer - clients message input
 * @param commandType - message type
 * @param username - clients username
 * @param displayname - clients display name
 * @param secret - clients secret
 * @param counterForBye - counter which provides sending bye from state Auth,
 * should be at least 1(client should authenticate at least one time to send bye)
 */
void processAuthentication(Client& obj, char* buffer, string& commandType, string& username, string& displayname, string& secret, int& counterForBye);
/** brief
 *
 * @param buffer - clients message input
 * @param commandType - message type
 * @param channelId - channel which client wants to join
 * @param displayname - clients display name
 * @return
 */
bool joinChannelProcess(Client& obj,char* buffer, string& commandType, string& channelId, string& displayname);

#endif //IPKPROJEKTOOP_CONNECTION_H
