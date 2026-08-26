#pragma once

namespace Kiwi
{
        /// This function should be called only if an unrecoverable error happened.
        /// If there is a process running, then this process is immediately aborted.
        /// Otherwise, the interrupts are canceled and the CPU is idling forever.
        void panic(const char *fmt, ...);
} // namespace Kiwi