#include <lib/args.hpp>
#include <lib/logging.hpp>
#include <lib/print.hpp>

namespace kernel::lib::log {

void status(const char *fmt, ...)
{
        va_list args;
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
        printf("...");
}

void ok()
{
        println(" ok");
}

void failed()
{
        println(" failed");
}

} /* namespace kernel::lib::log */