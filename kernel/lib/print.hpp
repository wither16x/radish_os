#pragma once

#include <lib/args.hpp>

namespace kernel::lib {

void putchar(int ch);
void print_string(const char *s);

void vprintf(const char *fmt, va_list args);
void printf(const char *fmt, ...);
void println(const char *fmt, ...);

} /* namespace kernel::lib */