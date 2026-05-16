#include <cpu/assembly.hpp>
#include <lib/args.hpp>
#include <lib/logging.hpp>
#include <lib/print.hpp>
#include <panic.hpp>

using namespace kernel::lib;

namespace kernel {

void panic(const char *fmt, ...)
{
        println("==================== KERNEL PANIC ====================");
        va_list args;
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
        println("");

        log::status("idling");
        while (true)
                cpu::hlt();
}

} /* namespace kernel */