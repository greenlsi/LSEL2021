#ifndef FSM_REBOUND_INTERNAL_H
#define FSM_REBOUND_INTERNAL_H

#include "fsm.h"

enum {
    READY,
    WAIT
};

int fsm_rebound_check(fsm_t* f);

#endif