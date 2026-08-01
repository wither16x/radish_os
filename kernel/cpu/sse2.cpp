#include <cpu/sse2.hpp>

namespace kernel::cpu {

namespace {

extern "C" void __enable_sse2();

} /* anonymous namespace */

void enable_sse2()
{
        __enable_sse2();
}

} /* namespace kernel::cpu */