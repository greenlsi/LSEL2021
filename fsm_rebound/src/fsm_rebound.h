#ifndef FSM_REBOUND_H
#define FSM_REBOUND_H

#include "fsm.h"

struct _fsm_rebound_t {
    fsm_t fsm;
};
typedef struct _fsm_rebound_t fsm_rebound_t;

void fsm_rebound_init(fsm_rebound_t* f);


#endif // FSM_REBOUND_H
