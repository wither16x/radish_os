#pragma once

namespace kernel {

// This function should be called only if an unrecoverable error happened.
// It disables interrupts and completely hangs the CPU.
[[gnu::noreturn]] void panic(const char *fmt, ...);

} /* namespace kernel */