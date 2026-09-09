/**
 * @file Implementation details of the generic timer driver tests.
 */
#include <cstdint>

#include "driver/timer/test_generic.h"
#include "yrgo/test/test.h"

namespace driver::timer::test
{
namespace
{
// -----------------------------------------------------------------------------
void tick(Interface& timer, const std::uint32_t count) noexcept
{
    for (std::uint32_t i{}; i < count; ++i)
    {
        timer.tick();
    }
}

// -----------------------------------------------------------------------------
[[nodiscard]] std::uint32_t ticksPerTimeout(const Interface& timer) noexcept
{
    return timer.timeout_ms();
}

// -----------------------------------------------------------------------------
void expectPositiveTimeout(const Interface& timer) { EXPECT_TRUE(0U < timer.timeout_ms()); }
} // namespace

// -----------------------------------------------------------------------------
void testInitialState(Interface& timer, const std::uint16_t expectedTimeout_ms,
                      const bool expectedRunning)
{
    EXPECT_EQ(timer.timeout_ms(), expectedTimeout_ms);
    EXPECT_EQ(timer.isRunning(), expectedRunning);
    EXPECT_FALSE(timer.hasTimedOut());
}

// -----------------------------------------------------------------------------
void testStartAndStop(Interface& timer)
{
    timer.start();
    EXPECT_TRUE(timer.isRunning());

    // Expect a repeated start to keep the timer running.
    timer.start();
    EXPECT_TRUE(timer.isRunning());

    timer.stop();
    EXPECT_FALSE(timer.isRunning());

    // Expect a repeated stop to keep the timer stopped.
    timer.stop();
    EXPECT_FALSE(timer.isRunning());
}

// -----------------------------------------------------------------------------
void testToggle(Interface& timer)
{
    constexpr std::uint8_t toggleCount{5U};

    // Start out stopped, so that the expected state after each toggle is known.
    timer.stop();

    // Expect the running state to alternate on every toggle.
    for (std::uint8_t i{}; i < toggleCount; ++i)
    {
        const bool expectedRunning{0U == (i % 2U)};
        timer.toggle();
        EXPECT_EQ(timer.isRunning(), expectedRunning);
    }
}

// -----------------------------------------------------------------------------
void testTickWhileStopped(Interface& timer)
{
    constexpr std::uint32_t timeoutCount{2U};

    // Clear the elapsed time via start(), then stop the timer.
    timer.start();
    timer.stop();

    // Expect no timeout, no matter how many ticks a stopped timer is given.
    tick(timer, timeoutCount * ticksPerTimeout(timer));
    EXPECT_FALSE(timer.isRunning());
    EXPECT_FALSE(timer.hasTimedOut());
}

// -----------------------------------------------------------------------------
void testNoTimeoutBeforeConfiguredTicks(Interface& timer)
{
    expectPositiveTimeout(timer);
    timer.start();

    // Expect no timeout one tick short of the configured timeout.
    tick(timer, ticksPerTimeout(timer) - 1U);
    EXPECT_FALSE(timer.hasTimedOut());
}

// -----------------------------------------------------------------------------
void testTimeoutAfterConfiguredTicks(Interface& timer)
{
    timer.start();

    // Expect a timeout after exactly the configured number of ticks.
    tick(timer, ticksPerTimeout(timer));
    EXPECT_TRUE(timer.hasTimedOut());
}

// -----------------------------------------------------------------------------
void testTimeoutIsClearedWhenRead(Interface& timer)
{
    timer.start();
    tick(timer, ticksPerTimeout(timer));
    EXPECT_TRUE(timer.hasTimedOut());

    // Expect the read above to have cleared the timeout, without any ticks in between.
    EXPECT_FALSE(timer.hasTimedOut());
}

// -----------------------------------------------------------------------------
void testTimeoutIsPeriodic(Interface& timer)
{
    constexpr std::uint8_t periodCount{3U};
    timer.start();

    // Expect a new timeout every configured number of ticks, without restarting the timer.
    for (std::uint8_t i{}; i < periodCount; ++i)
    {
        tick(timer, ticksPerTimeout(timer));
        EXPECT_TRUE(timer.hasTimedOut());
    }
}

// -----------------------------------------------------------------------------
void testStartClearsElapsedTime(Interface& timer)
{
    expectPositiveTimeout(timer);

    // Almost time out, then restart the timer.
    timer.start();
    tick(timer, ticksPerTimeout(timer) - 1U);
    timer.start();

    // Expect the elapsed time to have been cleared, i.e. one tick short of a timeout again.
    tick(timer, ticksPerTimeout(timer) - 1U);
    EXPECT_FALSE(timer.hasTimedOut());

    tick(timer, 1U);
    EXPECT_TRUE(timer.hasTimedOut());
}

// -----------------------------------------------------------------------------
void testToggleClearsElapsedTime(Interface& timer)
{
    expectPositiveTimeout(timer);

    // Almost time out, then toggle twice, so that the timer is running again.
    timer.start();
    tick(timer, ticksPerTimeout(timer) - 1U);
    timer.toggle();
    timer.toggle();
    EXPECT_TRUE(timer.isRunning());

    // Expect the elapsed time to have been cleared, i.e. one tick short of a timeout again.
    tick(timer, ticksPerTimeout(timer) - 1U);
    EXPECT_FALSE(timer.hasTimedOut());

    tick(timer, 1U);
    EXPECT_TRUE(timer.hasTimedOut());
}

// -----------------------------------------------------------------------------
void testStopKeepsReachedTimeout(Interface& timer)
{
    timer.start();
    tick(timer, ticksPerTimeout(timer));

    // Expect the timeout reached above to survive the stop, since stop() only freezes the
    // elapsed time.
    timer.stop();
    EXPECT_FALSE(timer.isRunning());
    EXPECT_TRUE(timer.hasTimedOut());
}

// -----------------------------------------------------------------------------
void testExtraTicksKeepTimeout(Interface& timer)
{
    // The number of ticks that wraps a 16-bit elapsed time back to zero.
    constexpr std::uint32_t counterWrapTicks{65536U};

    timer.start();
    tick(timer, counterWrapTicks);
    EXPECT_TRUE(timer.hasTimedOut());
}

// -----------------------------------------------------------------------------
void testInstancesAreIndependent(Interface& first, Interface& second)
{
    // Put both timers in a known state, then run the first one only.
    second.start();
    second.stop();
    first.start();
    tick(first, ticksPerTimeout(first));

    // Expect the second timer to be untouched by the ticks given to the first one.
    EXPECT_TRUE(first.hasTimedOut());
    EXPECT_FALSE(second.isRunning());
    EXPECT_FALSE(second.hasTimedOut());
}

// -----------------------------------------------------------------------------
void runAllTests(Interface& timer)
{
    testStartAndStop(timer);
    testToggle(timer);
    testTickWhileStopped(timer);
    testNoTimeoutBeforeConfiguredTicks(timer);
    testTimeoutAfterConfiguredTicks(timer);
    testTimeoutIsClearedWhenRead(timer);
    testTimeoutIsPeriodic(timer);
    testStartClearsElapsedTime(timer);
    testToggleClearsElapsedTime(timer);
    testStopKeepsReachedTimeout(timer);
    testExtraTicksKeepTimeout(timer);
}
} // namespace driver::timer::test
