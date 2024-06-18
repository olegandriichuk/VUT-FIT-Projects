//
// Created by oleg on 16.4.24.
//

#ifndef IPKSERVEROOP_SERVERREPLIES_H
#define IPKSERVEROOP_SERVERREPLIES_H
#include "string"
#include "vector"
#include "cstring"
#include "sstream"
#define BUFFER_SIZE 1024
using namespace std;
void constructPositiveAuthReply(char* buffer);


void constructNegativeAuthReply(char* buffer);

void constructPositiveJoinReply(char* buffer);
void constructNegativeJoinReply(char* buffer);

void constructError(char* buffer);

void constructMessage(char* buffer, const std::string& displayName, const std::string& isWord, const std::string& fromWord, const std::string& msgWord);

std::vector<std::string> parseLine(const char* buffer);
void constructJoin(char* buffer, const std::string& channelId, const std::string& Displayname);

void constructChannelLeaving(char* buffer, const std::string& channelId, const std::string& Displayname);
#endif //IPKSERVEROOP_SERVERREPLIES_H
