#include <string.h>

#include "unity.h"

#include "fsm.h"
#include "fsm_rebound.h"
#include "fsm_rebound_internal.h"

#include "mock_client.h"
#include "mock_timer.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_fsm_rebound_fsmInitFillsStructWithSomething(void)
{
    fsm_rebound_t zero;
    fsm_rebound_t f;
    bzero(&zero, sizeof(fsm_rebound_t));
    bzero(&f, sizeof(fsm_rebound_t));

    fsm_rebound_init(&f, NULL);
    TEST_ASSERT(memcmp(&zero, &f, sizeof(fsm_rebound_t)) != 0);
}

void test_fsm_rebound_fsmFireCallsCheckWhenReady(void)
{
    fsm_rebound_t f;

    custom_check_ExpectAndReturn(0);

    fsm_rebound_init(&f, custom_check);

    TEST_ASSERT(f.fsm.current_state == READY);
    fsm_fire((fsm_t*)(&f));
}

void test_fsm_rebound_fsmFireNoCallsCheckWhenWait(void)
{
    fsm_rebound_t f;

    timer_get_tick_IgnoreAndReturn(0);

    fsm_rebound_init(&f, custom_check);
    f.fsm.current_state = WAIT;
    fsm_fire((fsm_t*)(&f));
}

void test_fsm_rebound_fsmFireFollowsTransitionWhenReadyAndCheckIsTrue(void)
{
    fsm_rebound_t f;

    custom_check_IgnoreAndReturn(1);

    fsm_rebound_init(&f, custom_check);
    fsm_fire((fsm_t*)(&f));

    TEST_ASSERT(f.fsm.current_state == WAIT);
}

void test_fsm_rebound_fsmFireDontFollowTransitionWhenReadyAndCheckIsFalse(void)
{
    fsm_rebound_t f;

    custom_check_IgnoreAndReturn(0);

    fsm_rebound_init(&f, custom_check);
    fsm_fire((fsm_t*)(&f));

    TEST_ASSERT(f.fsm.current_state == READY);
}

void test_fsm_rebound_fsmFireCallsGetTickWhenWait(void)
{
    fsm_rebound_t f;

    timer_get_tick_ExpectAndReturn(0);

    fsm_rebound_init(&f, NULL);
    f.fsm.current_state = WAIT;

    fsm_fire((fsm_t*)(&f));
}

void test_fsm_rebound_fsmFireNoCallsGetTickWhenReady(void)
{
    fsm_rebound_t f;

    fsm_rebound_init(&f, NULL);
    TEST_ASSERT(f.fsm.current_state == READY);
    fsm_fire((fsm_t*)(&f));
}
