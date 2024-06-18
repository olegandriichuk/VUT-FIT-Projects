//
// Created by oleg on 20.4.24.
//

#include "interrupt.h"
#include <iostream>

#include <csignal>


 std::atomic<bool> force_quit(false);
// Handler function for C-c
void cc_handler(sig_atomic_t sig){
    if (sig == SIGINT){
        force_quit = true;
    }
}

void set_cc_handler(struct sigaction *sigIntHandler){
    // Initialize struct for C-c signal handling
    sigIntHandler->sa_handler = cc_handler;
    sigemptyset(&sigIntHandler->sa_mask);
    sigIntHandler->sa_flags = 0;

    sigaction(SIGINT, sigIntHandler, NULL);
}