//
// Created by oleg on 31.3.24.
//

#ifndef IPKPROJEKTOOP_CONSTRUCTMESSAGES_H
#define IPKPROJEKTOOP_CONSTRUCTMESSAGES_H
#include "Client.h"
#include "dataValidation.h"
/** brief
 * contruct a formatted message from a positive incoming REPLY message
 * @param buffer - contains message from server
 */
void constructPositiveReply(char* buffer);

/** brief
 * contruct a formatted message from a negative incoming REPLY message
 * @param buffer - contains message from server
 */

void constructNegativeReply(char* buffer);
/** brief
 * contruct a formatted error message from an incoming ERR message
 * @param buffer - contains message from server
 */
void convertErrorMessage(char* buffer);

/** brief
 * contruct a formatted  message from an incoming MSG message
 * @param buffer - contains message from server
 */
void convertMessage(char* buffer);
/** brief
 * construct a formatted AUTH message from a client's AUTH message
 * @param buffer - contains auth message from client
 * @param Username - client's Username
 * @param Displayname - client's Display name
 * @param secret - client's Secret
 */
void  constructAuth(char* buffer, const string& Username, const string& Displayname, const string& secret);

/** brief
 * construct a formatted JOIN message from a client's JOIN message
 * @param buffer - contains join  message from client
 * @param channelId - channel which client wants to join
 * @param Displayname  - client's Display name
 */
void  constructJoin(char* buffer, const string& channelId, const string& Displayname);
/** brief
 * construct a formatted MSG message from a client's  message
 * @param buffer - contains simple message from client
 * @param Displayname  - client's Display name
 */
void  constructMessage(char* buffer, const string& Displayname);
/** brief
 * construct a formatted Error message from a client's error message
 * @param buffer - contains simple message from client
 * @param Displayname - client's Display name
 */


void  constructErrorFromClient(char* buffer, const string& Displayname);
/** brief
 * helps find out message type
 * @param message - client`s or server message
 * @return message type
 */
string extractAndCheckMessage(const std::string& message);

string extractSecondWord(const std::string& message);

string extractThirdWord(const std::string& message);

string extractFourthWord(const std::string& message);

void constructError(char* buffer, const std::string& Displayname);
#endif //IPKPROJEKTOOP_CONSTRUCTMESSAGES_H
