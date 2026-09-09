/**
 * @file Unit tests for the timer stub driver.
 */
#include <cstdint>
#include <type_traits>

#include "driver/timer/stub.h"
#include "driver/timer/test_generic.h"
#include "yrgo/test/test.h"

namespace driver::timer
{
namespace
{
/** Timeout used by the tests that don't test a particular timeout value. */
constexpr std::uint16_t Timeout_ms{100U};

/** Shortest timeout the stub can be configured with. */
constexpr std::uint16_t MinTimeout_ms{1U};

/** Longest timeout the stub can be configured with, given its 16-bit timeout. */
constexpr std::uint16_t MaxTimeout_ms{65535U};

/**
 * @brief Test the state of a newly constructed timer.
 */
TEST(TimerStub, InitialState)
{
    Stub timer{Timeout_ms};
    test::testInitialState(timer, Timeout_ms, false);
}

/**
 * @brief Test the state of a timer started at construction.
 */
TEST(TimerStub, InitialStateWhenStarted)
{
    Stub timer{Timeout_ms, true};
    test::testInitialState(timer, Timeout_ms, true);
}

/**
 * @brief Test starting and stopping the timer.
 */
TEST(TimerStub, StartAndStop)
{
    Stub timer{Timeout_ms};
    test::testStartAndStop(timer);
}

/**
 * @brief Test toggling the timer.
 */
TEST(TimerStub, Toggle)
{
    Stub timer{Timeout_ms};
    test::testToggle(timer);
}

/**
 * @brief Test that a stopped timer doesn't advance.
 */
TEST(TimerStub, TickWhileStopped)
{
    Stub timer{Timeout_ms};
    test::testTickWhileStopped(timer);
}

/**
 * @brief Test that the timer doesn't time out too early.
 */
TEST(TimerStub, NoTimeoutBeforeConfiguredTicks)
{
    Stub timer{Timeout_ms};
    test::testNoTimeoutBeforeConfiguredTicks(timer);
}

/**
 * @brief Test that the timer times out on time.
 */
TEST(TimerStub, TimeoutAfterConfiguredTicks)
{
    Stub timer{Timeout_ms};
    test::testTimeoutAfterConfiguredTicks(timer);
}

/**
 * @brief Test that a reported timeout is cleared.
 */
TEST(TimerStub, TimeoutIsClearedWhenRead)
{
    Stub timer{Timeout_ms};
    test::testTimeoutIsClearedWhenRead(timer);
}

/**
 * @brief Test that the timer restarts by itself.
 */
TEST(TimerStub, TimeoutIsPeriodic)
{
    Stub timer{Timeout_ms};
    test::testTimeoutIsPeriodic(timer);
}

/**
 * @brief Test that start() clears the elapsed time.
 */
TEST(TimerStub, StartClearsElapsedTime)
{
    Stub timer{Timeout_ms};
    test::testStartClearsElapsedTime(timer);
}

/**
 * @brief Test that toggle() clears the elapsed time.
 */
TEST(TimerStub, ToggleClearsElapsedTime)
{
    Stub timer{Timeout_ms};
    test::testToggleClearsElapsedTime(timer);
}

/**
 * @brief Test that stop() keeps a timeout that has already occurred.
 */
TEST(TimerStub, StopKeepsReachedTimeout)
{
    Stub timer{Timeout_ms};
    test::testStopKeepsReachedTimeout(timer);
}

/**
 * @brief Test that a timeout survives further ticks.
 */
TEST(TimerStub, ExtraTicksKeepTimeout)
{
    Stub timer{Timeout_ms};
    test::testExtraTicksKeepTimeout(timer);
}

/**
 * @brief Test that two timers don't share any state.
 */
TEST(TimerStub, InstancesAreIndependent)
{
    Stub first{Timeout_ms};
    Stub second{Timeout_ms};
    test::testInstancesAreIndependent(first, second);
}

/**
 * @brief Test the default value of the constructor's start parameter.
 *
 *        Expect a timer to be stopped unless it is explicitly started, and to stay stopped even
 *        when ticked.
 */
TEST(TimerStub, StoppedByDefault)
{
    Stub timer{Timeout_ms};
    EXPECT_FALSE(timer.isRunning());

    timer.tick();
    EXPECT_FALSE(timer.isRunning());
}

/**
 * @brief Test that a timer started at construction counts from zero.
 *
 *        Expect a full timeout worth of ticks to be required, i.e. the constructor must clear
 *        the elapsed time and not just set the running state.
 */
TEST(TimerStub, StartedAtConstructionCountsFromZero)
{
    Stub timer{Timeout_ms, true};

    // Tick to one tick short of the timeout, without restarting the timer first.
    for (std::uint16_t i{}; i < Timeout_ms - 1U; ++i)
    {
        timer.tick();
    }
    EXPECT_FALSE(timer.hasTimedOut());

    timer.tick();
    EXPECT_TRUE(timer.hasTimedOut());
}

/**
 * @brief Test the shortest timeout the stub accepts.
 *
 *        Expect a timer configured with 1 ms to time out on a single tick.
 */
TEST(TimerStub, MinimumTimeout)
{
    Stub timer{MinTimeout_ms, true};
    EXPECT_FALSE(timer.hasTimedOut());

    timer.tick();
    EXPECT_TRUE(timer.hasTimedOut());
}

/**
 * @brief Test the shortest timeout against the generic checks.
 */
TEST(TimerStub, MinimumTimeoutPassesGenericTests)
{
    Stub timer{MinTimeout_ms};
    test::runAllTests(timer);
}

/**
 * @brief Test the longest timeout the stub accepts.
 */
TEST(TimerStub, MaximumTimeout)
{
    Stub timer{MaxTimeout_ms};
    test::testNoTimeoutBeforeConfiguredTicks(timer);
    test::testTimeoutAfterConfiguredTicks(timer);
}

/**
 * @brief Test that two timers keep their own timeouts.
 */
TEST(TimerStub, InstancesKeepTheirOwnTimeout)
{
    constexpr std::uint16_t shortTimeout_ms{10U};
    constexpr std::uint16_t longTimeout_ms{1000U};

    Stub first{shortTimeout_ms};
    Stub second{longTimeout_ms};

    EXPECT_EQ(first.timeout_ms(), shortTimeout_ms);
    EXPECT_EQ(second.timeout_ms(), longTimeout_ms);

    // Expect the shorter timer to time out without affecting the longer one.
    test::testInstancesAreIndependent(first, second);
    EXPECT_EQ(second.timeout_ms(), longTimeout_ms);
}

/**
 * @brief Test that the stub is usable through the interface.
 *
 *        Expect calls made through an Interface reference to reach the stub itself, which is how
 *        the stub is used as a test double for code that depends on a timer.
 */
TEST(TimerStub, UsageViaInterface)
{
    Stub stub{Timeout_ms};
    Interface& timer{stub};

    EXPECT_EQ(timer.timeout_ms(), Timeout_ms);

    timer.start();
    EXPECT_TRUE(stub.isRunning());

    timer.stop();
    EXPECT_FALSE(stub.isRunning());
}
} // namespace
} // namespace driver::timer
