#include <cpu/assembly.hpp>
#include <lib/print.hpp>

using namespace kernel;
using namespace kernel::lib;

extern "C" [[gnu::noreturn]] void exception_handler()
{
        println("CPU exception");
        while (true)
                cpu::hlt();
}