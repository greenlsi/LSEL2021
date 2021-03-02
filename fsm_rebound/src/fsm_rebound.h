#ifndef FSM_REBOUND_H
#define FSM_REBOUND_H

#include "fsm.h"

typedef int (*fsm_rebound_check_func_t) (void);

struct _fsm_rebound_t {
    fsm_t fsm;
    fsm_rebound_check_func_t check;
};
typedef struct _fsm_rebound_t fsm_rebound_t;


void fsm_rebound_init(fsm_rebound_t* f, fsm_rebound_check_func_t check);


#endif // FSM_REBOUND_H
