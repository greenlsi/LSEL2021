#ifndef FSM_REBOUND_H
#define FSM_REBOUND_H

#include <stdint.h>

#include "fsm.h"

typedef int (*fsm_rebound_check_func_t) (void);
typedef void (*fsm_rebound_reset_func_t) (void);

struct _fsm_rebound_t {
    fsm_t fsm;
    fsm_rebound_check_func_t check;
    fsm_rebound_reset_func_t reset;
    uint32_t time_timeout;
    uint32_t next_timeout;
};
typedef struct _fsm_rebound_t fsm_rebound_t;

void fsm_rebound_init(fsm_rebound_t* f, fsm_rebound_check_func_t check, fsm_rebound_reset_func_t reset, uint32_t timeout);


#endif // FSM_REBOUND_H
