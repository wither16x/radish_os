#pragma once

#include <lib/typing.hpp>

namespace kernel {

namespace lib {

char *itoa(isize n, int base);
char *utoa(usize n, int base);

} /* namespace lib */

} /* namespace kernel */