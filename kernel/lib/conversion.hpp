#pragma once

#include <lib/typing.hpp>

namespace kernel::lib {

char *itoa(isize n, int base);
char *utoa(usize n, int base);
usize atoi(const char *s, int base);
// base 10 only
usize ctoi(char ch);

} /* namespace kernel::lib */