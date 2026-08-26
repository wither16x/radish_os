#pragma once

#include <lib/typing.hpp>

namespace Kiwi::Drivers::Pit
{
        /// Initialize the PIT by automatically setting the number
        /// of ticks per seconds.
        void init();
        /// Add a tic to the counter.
        void tick();
        /// Sleep `ms` miliseconds.
        void sleep(Lib::u64 ms);
        /// Return the current value of the tic counter.
        Lib::u64 getTics();
        // Return the current value of the seconds counter.
        Lib::u64 getSeconds();
} // namespace Kiwi::Drivers::Pit