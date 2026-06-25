#pragma once

#include <lib/typing.hpp>

namespace kernel::lib {

/// Get a string from an unsigned integer.
char *itoa(isize n, int base);
/// Get a string from a signed integer.
char *utoa(usize n, int base);
/// Get an unsigned integer from a string.
usize atoi(const char *s, int base);
/// Get an unsigned integer from a single character. Note that
/// only base 10 is supported. 
usize ctoi(char ch);

} /* namespace kernel::lib */