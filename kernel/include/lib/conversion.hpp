#pragma once

#include <lib/typing.hpp>

namespace kernel::lib {

/// Get a string from an unsigned integer.
char *itoa(isize n, char *str, int base);
/// Get a string from a signed integer.
char *utoa(usize n, char *str, int base);
/// Get an unsigned integer from a string.
usize atoi(const char *s, int base, usize length);
/// Get an unsigned integer from a single character. Note that
/// only base 10 is supported. 
usize ctoi(char ch);
/// Get a floating-point value from a string.
double atof(const char *s);
/// Get a string from a floating-point value.
char *ftoa(double n, char *buf);

} /* namespace kernel::lib */