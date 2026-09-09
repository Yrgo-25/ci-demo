/**
 * @file Firmware demo.
 */
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <thread>

#include "driver/timer/stub.h"

namespace
{
// -----------------------------------------------------------------------------
void runTimer(driver::timer::Interface& timer, const std::uint32_t iterations) noexcept
{
    constexpr std::uint8_t tick_ms{1U};
    timer.start();

    for (std::uint32_t i{}; i < iterations; ++i)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(tick_ms));
        timer.tick();

        if (timer.hasTimedOut()) { std::printf("Timeout after %u ms!\n", timer.timeout_ms()); }
    }
}
} // namespace

/**
 * @brief Application entry point.
 *
 * @return 0 on termination of the program.
 */
int main()
{
    constexpr std::uint32_t iterations{5000U};
    constexpr std::uint16_t timeout_ms{500U};
    driver::timer::Stub timer{timeout_ms};
    runTimer(timer, iterations);
    return 0;
}
