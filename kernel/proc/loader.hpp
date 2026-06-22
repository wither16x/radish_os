#pragma once

#include <lib/string.hpp>
#include <lib/typing.hpp>

namespace kernel::proc {

// pml4t: pml4 table of the process
// hhdm: hhdm offset
void load_program(lib::u64 *pml4t, const lib::String &path, lib::uptr address, lib::uptr hhdm);

} /* namespace kernel::proc */