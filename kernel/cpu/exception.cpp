#include <cpu/assembly.hpp>
#include <panic.hpp>

using namespace kernel;
using namespace kernel::lib;

extern "C" [[gnu::noreturn]] void exception_handler()
{
        panic("CPU exception");
}