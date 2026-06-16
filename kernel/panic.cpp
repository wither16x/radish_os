#include <cpu/assembly.hpp>
#include <lib/args.hpp>
#include <lib/logging.hpp>
#include <lib/print.hpp>
#include <panic.hpp>

namespace kernel {

using lib::vprintf, lib::println;
using lib::log::logger;

void panic(const char *fmt, ...)
{
        cpu::cli();

        logger.set_context("panic handler");

        println("\r\n==================== KERNEL PANIC ====================");
        va_list args;
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
        println("");

        logger.info("idling");
        while (true)
                cpu::hlt();
}

} /* namespace kernel */