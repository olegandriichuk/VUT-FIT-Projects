//
// Created by oleg on 20.4.24.
//

#ifndef IPKSERVEROOP_INTERRUPT_H
#define IPKSERVEROOP_INTERRUPT_H
#include "Server.h"
extern std::atomic<bool> force_quit;
void cc_handler(int sig);
void set_cc_handler(struct sigaction *sigIntHandler);
#endif //IPKSERVEROOP_INTERRUPT_H
