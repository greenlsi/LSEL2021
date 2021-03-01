#include <string.h>

#include "unity.h"

#include "fsm.h"
#include "fsm_rebound.h"
#include "mock_fsm_rebound_internal.h"

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

    fsm_rebound_init(&f);
    TEST_ASSERT(memcmp(&zero, &f, sizeof(fsm_rebound_t)) != 0);
}

void test_fsm_rebound_fsmFireCallsCheckWhenReady(void)
{
    fsm_rebound_t f;

    fsm_rebound_check_ExpectAnyArgsAndReturn(0);

    fsm_rebound_init(&f);

    TEST_ASSERT(f.fsm.current_state == READY);
    fsm_fire((fsm_t*)(&f));
}

void test_fsm_rebound_fsmFireNoCallsCheckWhenWait(void)
{
    fsm_rebound_t f;

    fsm_rebound_init(&f);
    f.fsm.current_state = WAIT;
    fsm_fire((fsm_t*)(&f));
}

void test_fsm_rebound_fsmFireFollowsTransitionWhenReadyAndCheckIsTrue(void)
{
    fsm_rebound_t f;

    fsm_rebound_check_IgnoreAndReturn(1);

    fsm_rebound_init(&f);
    fsm_fire((fsm_t*)(&f));

    TEST_ASSERT(f.fsm.current_state == WAIT);
}

void test_fsm_rebound_fsmFireDontFollowTransitionWhenReadyAndCheckIsFalse(void)
{
    fsm_rebound_t f;

    fsm_rebound_check_IgnoreAndReturn(0);

    fsm_rebound_init(&f);
    fsm_fire((fsm_t*)(&f));

    TEST_ASSERT(f.fsm.current_state == READY);
}