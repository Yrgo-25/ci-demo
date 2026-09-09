/**
 * @file Generic unit tests for timer drivers.
 */
#pragma once

#include <cstdint>

#include "driver/timer/interface.h"

namespace driver::timer::test
{
/**
 * @brief Test the state of a newly constructed timer.
 *
 *        Expect the configured timeout to be reported, the running state to match the one
 *        requested at construction, and the timer not to have timed out.
 *
 * @param[in] timer Timer to test. Must be newly constructed.
 * @param[in] expectedTimeout_ms Timeout passed to the constructor.
 * @param[in] expectedRunning True if the timer was started at construction.
 */
void testInitialState(Interface& timer, std::uint16_t expectedTimeout_ms, bool expectedRunning);

/**
 * @brief Test starting and stopping the timer.
 *
 * @param[in] timer Timer to test.
 */
void testStartAndStop(Interface& timer);

/**
 * @brief Test toggling the timer.
 *
 * @param[in] timer Timer to test.
 */
void testToggle(Interface& timer);

/**
 * @brief Test that a stopped timer doesn't advance.
 *
 *        Expect no timeout from a stopped timer, no matter how many times tick() is called.
 *
 * @param[in] timer Timer to test.
 */
void testTickWhileStopped(Interface& timer);

/**
 * @brief Test that the timer doesn't time out too early.
 *
 *        Expect no timeout after timeout_ms() - 1 calls to tick().
 *
 * @param[in] timer Timer to test.
 */
void testNoTimeoutBeforeConfiguredTicks(Interface& timer);

/**
 * @brief Test that the timer times out on time.
 *
 *        Expect a timeout after exactly timeout_ms() calls to tick().
 *
 * @param[in] timer Timer to test.
 */
void testTimeoutAfterConfiguredTicks(Interface& timer);

/**
 * @brief Test that a reported timeout is cleared.
 *
 *        Expect the timeout to be gone when read a second time, without any calls to tick()
 *        in between.
 *
 * @param[in] timer Timer to test.
 */
void testTimeoutIsClearedWhenRead(Interface& timer);

/**
 * @brief Test that the timer restarts by itself.
 *
 *        Expect a new timeout after another timeout_ms() calls to tick(), without restarting
 *        the timer in between.
 *
 * @param[in] timer Timer to test.
 */
void testTimeoutIsPeriodic(Interface& timer);

/**
 * @brief Test that start() clears the elapsed time.
 *
 *        Expect a restart in the middle of a measurement to postpone the timeout by a full
 *        timeout_ms() ticks.
 *
 * @param[in] timer Timer to test.
 */
void testStartClearsElapsedTime(Interface& timer);

/**
 * @brief Test that toggle() clears the elapsed time.
 *
 *        Expect toggling twice in the middle of a measurement to postpone the timeout by a full
 *        timeout_ms() ticks.
 *
 * @param[in] timer Timer to test.
 */
void testToggleClearsElapsedTime(Interface& timer);

/**
 * @brief Test that stop() keeps a timeout that has already occurred.
 *
 *        Expect a timeout reached before stop() to still be reported afterwards, since stop()
 *        freezes the elapsed time rather than clearing it.
 *
 * @param[in] timer Timer to test.
 */
void testStopKeepsReachedTimeout(Interface& timer);

/**
 * @brief Test that a timeout survives further ticks.
 *
 *        Expect a timeout after ticking exactly as many times as it takes to wrap a 16-bit
 *        elapsed time back to zero, i.e. the elapsed time must stop at the timeout rather than
 *        keep counting past it.
 *
 * @param[in] timer Timer to test.
 */
void testExtraTicksKeepTimeout(Interface& timer);

/**
 * @brief Test that two timers are independent of each other.
 *
 *        Expect running one timer to its timeout to leave the other one untouched.
 *
 * @param[in] first  Timer to run to its timeout.
 * @param[in] second Timer expected to be unaffected.
 */
void testInstancesAreIndependent(Interface& first, Interface& second);

/**
 * @brief Run every test that operates on a single timer.
 *
 * @param[in] timer Timer to test.
 */
void runAllTests(Interface& timer);

} // namespace driver::timer::test
