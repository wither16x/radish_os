#pragma once

#include <lib/typing.hpp>

namespace kernel {

lib::u64 *get_kernel_pml4t();
void set_kernel_pml4t(lib::u64 *pml4t);

} /* namespace kernel */