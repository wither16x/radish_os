#pragma once

#include <lib/typing.hpp>

namespace kernel::lib {

char *itoa(isize n, int base);
char *utoa(usize n, int base);

} /* namespace kernel::lib */