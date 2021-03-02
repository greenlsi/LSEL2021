#include <stddef.h>

#include "fsm_rebound.h"
#include "fsm_rebound_internal.h"

#include "timer.h"

static int
fsm_rebound_check(fsm_t* f)
{
    fsm_rebound_t* fp = (fsm_rebound_t*)f;

    if (fp->check) {
        return fp->check();
    }
    return 0;
}

static int
fsm_rebound_is_timeout(fsm_t* f)
{
    return timer_get_tick();
}


static fsm_trans_t
rebound_tt[] = {
    {READY, fsm_rebound_check, WAIT, NULL},
    {WAIT, fsm_rebound_is_timeout, READY, NULL},
    {-1, NULL, -1, NULL}
};

void fsm_rebound_init(fsm_rebound_t* f, fsm_rebound_check_func_t check)
{
    fsm_init((fsm_t*)f, rebound_tt);
    f->check = check;
}