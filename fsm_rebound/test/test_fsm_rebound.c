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

    fsm_rebound_init(&f, NULL, NULL, 0);
    TEST_ASSERT(memcmp(&zero, &f, sizeof(fsm_rebound_t)) != 0);
}

void test_fsm_rebound_fsmFireCallsCheckWhenReady(void)
{
    fsm_rebound_t f;

    custom_check_ExpectAndReturn(0);

    fsm_rebound_init(&f, custom_check, NULL, 0);

    TEST_ASSERT(f.fsm.current_state == READY);
    fsm_fire((fsm_t*)(&f));
}

void test_fsm_rebound_fsmFireNoCallsCheckWhenWait(void)
{
    fsm_rebound_t f;

    timer_get_tick_IgnoreAndReturn(0);

    fsm_rebound_init(&f, custom_check, NULL, 0);
    f.fsm.current_state = WAIT;
    fsm_fire((fsm_t*)(&f));
}

void test_fsm_rebound_fsmFireFollowsTransitionWhenReadyAndCheckIsTrue(void)
{
    fsm_rebound_t f;

    custom_check_IgnoreAndReturn(1);
    timer_get_tick_IgnoreAndReturn(0);

    fsm_rebound_init(&f, custom_check, NULL, 0);
    fsm_fire((fsm_t*)(&f));

    TEST_ASSERT(f.fsm.current_state == WAIT);
}

void test_fsm_rebound_fsmFireDontFollowTransitionWhenReadyAndCheckIsFalse(void)
{
    fsm_rebound_t f;

    custom_check_IgnoreAndReturn(0);

    fsm_rebound_init(&f, custom_check, NULL, 0);
    fsm_fire((fsm_t*)(&f));

    TEST_ASSERT(f.fsm.current_state == READY);
}

void test_fsm_rebound_fsmFireCallsGetTickWhenWait(void)
{
    fsm_rebound_t f;

    timer_get_tick_ExpectAndReturn(0);

    fsm_rebound_init(&f, NULL, NULL, 0);
    f.fsm.current_state = WAIT;

    fsm_fire((fsm_t*)(&f));
}

void test_fsm_rebound_fsmFireNoCallsGetTickWhenReady(void)
{
    fsm_rebound_t f;

    fsm_rebound_init(&f, NULL, NULL, 0);
    TEST_ASSERT(f.fsm.current_state == READY);
    fsm_fire((fsm_t*)(&f));
}

void test_fsm_rebound_fsmFireFollowsTransitionWhenWaitAndIsTimeout(void)
{
    fsm_rebound_t f;

    timer_get_tick_ExpectAndReturn(1);

    fsm_rebound_init(&f, NULL, NULL, 1);
    f.fsm.current_state = WAIT;
    f.next_timeout = 0;

    fsm_fire((fsm_t*)(&f));

    TEST_ASSERT(f.fsm.current_state == READY);
}

void test_fsm_rebound_fsmFireDontFollowTransitionWhenWaitAndIsNoTimeout(void)
{
    fsm_rebound_t f;

    custom_check_IgnoreAndReturn(1);
    timer_get_tick_ExpectAndReturn(0);

    fsm_rebound_init(&f, NULL, NULL, 1);
    f.fsm.current_state = WAIT;
    f.next_timeout = 1;
    fsm_fire((fsm_t*)(&f));

    TEST_ASSERT(f.fsm.current_state == WAIT);
}

void test_fsm_rebound_fsmFireDontFollowTransitionWhenWaitAndIsTimeoutWithTickNoZero(void)
{
    fsm_rebound_t f;

    custom_check_IgnoreAndReturn(1);
    timer_get_tick_ExpectAndReturn(5);

    fsm_rebound_init(&f, NULL, NULL, 1);
    f.fsm.current_state = WAIT;
    f.next_timeout = 5;
    fsm_fire((fsm_t*)(&f));

    TEST_ASSERT(f.fsm.current_state == READY);
}


void test_fsm_rebound_fsmFireDontFollowTransitionWhenWaitAndIsNoTimeoutWithTickNoZero(void)
{
    fsm_rebound_t f;

    custom_check_IgnoreAndReturn(1);
    timer_get_tick_ExpectAndReturn(5);

    fsm_rebound_init(&f, NULL, NULL, 1);
    f.fsm.current_state = WAIT;
    f.next_timeout = 6;
    fsm_fire((fsm_t*)(&f));

    TEST_ASSERT(f.fsm.current_state == WAIT);
}

void test_fsm_rebound_fsmFireReadyToWaitUpdatesTimeout(void)
{
    fsm_rebound_t f;

    custom_check_IgnoreAndReturn(1);
    timer_get_tick_ExpectAndReturn(5);

    fsm_rebound_init(&f, custom_check, NULL, 3);
    TEST_ASSERT(f.fsm.current_state == READY);
    f.next_timeout = 0;
    fsm_fire((fsm_t*)(&f));

    TEST_ASSERT_EQUAL(5+3, f.next_timeout);
}

void test_fsm_rebound_fsmFireWaitToReadyWhenLongTickPassed(void)
{
    fsm_rebound_t f;

    custom_check_IgnoreAndReturn(1);
    timer_get_tick_ExpectAndReturn(20);

    fsm_rebound_init(&f, custom_check, NULL, 5);
    f.fsm.current_state = WAIT;
    f.next_timeout = 10;
    fsm_fire((fsm_t*)(&f));

    TEST_ASSERT(f.fsm.current_state == READY);
}

void test_fsm_rebound_fsmFireWaitToWaitWhenLongTickPassedNotEnough(void)
{
    fsm_rebound_t f;

    custom_check_IgnoreAndReturn(1);
    timer_get_tick_ExpectAndReturn(20);

    fsm_rebound_init(&f, custom_check, NULL, 5);
    f.fsm.current_state = WAIT;
    f.next_timeout = 30;
    fsm_fire((fsm_t*)(&f));

    TEST_ASSERT(f.fsm.current_state == WAIT);
}

void test_fsm_rebound_fsmFireWaitToReadyCallsResetFunction(void)
{
    fsm_rebound_t f;

    timer_get_tick_ExpectAndReturn(20);
    custom_reset_Expect();

    fsm_rebound_init(&f, custom_check, custom_reset, 5);
    f.fsm.current_state = WAIT;
    f.next_timeout = 0;
    fsm_fire((fsm_t*)(&f));

    TEST_ASSERT(f.fsm.current_state == READY);
}

void test_fsm_rebound_fsmFireAntiReboundOf5Complete(void)
{
    fsm_rebound_t f;

    fsm_rebound_init(&f, custom_check, custom_reset, 5);
    TEST_ASSERT(f.fsm.current_state == READY);

    custom_check_ExpectAndReturn(0);
    fsm_fire(&(f.fsm));   
    TEST_ASSERT(f.fsm.current_state == READY);

    custom_check_ExpectAndReturn(1);
    timer_get_tick_ExpectAndReturn(20);
    fsm_fire(&(f.fsm));
    TEST_ASSERT(f.fsm.current_state == WAIT);

    timer_get_tick_ExpectAndReturn(22);
    fsm_fire(&(f.fsm));
    TEST_ASSERT(f.fsm.current_state == WAIT);

    timer_get_tick_ExpectAndReturn(24);
    fsm_fire(&(f.fsm));
    TEST_ASSERT(f.fsm.current_state == WAIT);

    timer_get_tick_ExpectAndReturn(26);
    custom_reset_Expect();
    fsm_fire(&(f.fsm));
    TEST_ASSERT(f.fsm.current_state == READY);

    custom_check_ExpectAndReturn(0);
    fsm_fire(&(f.fsm));   
    TEST_ASSERT(f.fsm.current_state == READY);

}