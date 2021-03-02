#include <stddef.h>

#include "fsm_rebound.h"
#include "fsm_rebound_internal.h"

#include "timer.h"

#define TIMEOUT (1)

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
    fsm_rebound_t* fp = (fsm_rebound_t*)f;
    return (timer_get_tick() >= fp->next_timeout);
}

static void
fsm_rebound_set_timeout(fsm_t* f)
{
    fsm_rebound_t* fp = (fsm_rebound_t*)f;
    fp->next_timeout = timer_get_tick() + fp->time_timeout;
}

static void
fsm_rebound_reset(fsm_t* f)
{
    fsm_rebound_t* fp = (fsm_rebound_t*)f;
    if (fp->reset) {
        fp->reset();
    }
}

static fsm_trans_t
rebound_tt[] = {
    {READY, fsm_rebound_check, WAIT, fsm_rebound_set_timeout},
    {WAIT, fsm_rebound_is_timeout, READY, fsm_rebound_reset},
    {-1, NULL, -1, NULL}
};

void fsm_rebound_init(fsm_rebound_t* f, fsm_rebound_check_func_t check, fsm_rebound_reset_func_t reset, uint32_t timeout)
{
    fsm_init((fsm_t*)f, rebound_tt);
    f->check = check;
    f->reset = reset;
    f->time_timeout = timeout;
}