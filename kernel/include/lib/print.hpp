#pragma once

#include <lib/args.hpp>

namespace Kiwi::Lib
{
        /// Output a character to an automatically chosen environment.
        void putchar(int ch);
        /// Output a string.
        void printString(const char *s);
        /// Output a formatted string.
        void vprintf(const char *fmt, va_list args);
        /// Output a formatted string using variadic parameters.
        void printf(const char *fmt, ...);
        /// Output a formatted string formatted by a newline.
        void println(const char *fmt, ...);
} // namespace Kiwi::Lib