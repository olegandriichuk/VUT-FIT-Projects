//
// Created by oleg on 31.3.24.
//

#ifndef IPKPROJEKTOOP_DATAVALIDATION_H
#define IPKPROJEKTOOP_DATAVALIDATION_H
#include "Client.h"

/** brief
 * check if messageContent is valid
 * @param messageContent - message from client
 * @return true if message Content is valid
 */
bool isValidMessageContent(const  char*  messageContent);

/** brief
 * check if channelId is valid
 * @param channelId - channel which client wants to join
 * @return true if channelId is valid
 */
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
#endif //IPKPROJEKTOOP_DATAVALIDATION_H
