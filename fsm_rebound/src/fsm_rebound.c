#include <stddef.h>

#include "fsm_rebound.h"
#include "fsm_rebound_internal.h"


static fsm_trans_t
rebound_tt[] = {
    {READY, fsm_rebound_check, WAIT, NULL},
    {-1, NULL, -1, NULL}
};

void fsm_rebound_init(fsm_rebound_t* f)
{
    fsm_init((fsm_t*)f, rebound_tt);
}