#pragma once

#include <lib/typing.hpp>

namespace kernel::mem::heap {

void init();
void *allocate(lib::usize n);
void free(void *p);

} /* namespace kernel::mem::heap */