#include <lib/memory.hpp>
#include <lib/args.hpp>
#include <lib/logging.hpp>
#include <lib/print.hpp>

namespace kernel::lib::log {

namespace {

const char *log_level_str[] = {
        "\033[37mdebug",
        "\033[36minfo",
        "\033[32mok",
        "\033[33mwarn",
        "\033[31merr",
        "\033[35mcrit"
};

} /* anonymous namespace */

// --------------------------------------------------
void Logger::set_context(this Logger &self, const char *ctx)
{
        memmove(self.context, ctx, MAX_CONTEXT_BYTES);
}
// --------------------------------------------------

// --------------------------------------------------
void Logger::va_log(this Logger &self, Logger::LogLevel lv, const char *fmt, va_list args)
{
        printf("[%s] %s\033[37m: ", self.context, log_level_str[static_cast<int>(lv)]);
        vprintf(fmt, args);
        printf("\r\n");
}
// --------------------------------------------------

// --------------------------------------------------
void Logger::log(this Logger &self, Logger::LogLevel lv, const char *fmt, ...)
{
        va_list args;
        va_start(args, fmt);
        self.va_log(lv, fmt, args);
        va_end(args);
}
// --------------------------------------------------

// --------------------------------------------------
void Logger::debug(this Logger &self, const char *fmt, ...)
{
        va_list args;
        va_start(args, fmt);
        self.va_log(LogLevel::Debug, fmt, args);
        va_end(args);
}
// --------------------------------------------------

// --------------------------------------------------
void Logger::info(this Logger &self, const char *fmt, ...)
{
        va_list args;
        va_start(args, fmt);
        self.va_log(LogLevel::Info, fmt, args);
        va_end(args);
}
// --------------------------------------------------

// --------------------------------------------------
void Logger::ok(this Logger &self, const char *fmt, ...)
{
        va_list args;
        va_start(args, fmt);
        self.va_log(LogLevel::Ok, fmt, args);
        va_end(args);
}
// --------------------------------------------------

// --------------------------------------------------
void Logger::warn(this Logger &self, const char *fmt, ...)
{
        va_list args;
        va_start(args, fmt);
        self.va_log(LogLevel::Warn, fmt, args);
        va_end(args);
}
// --------------------------------------------------

// --------------------------------------------------
void Logger::err(this Logger &self, const char *fmt, ...)
{
        va_list args;
        va_start(args, fmt);
        self.va_log(LogLevel::Err, fmt, args);
        va_end(args);
}
// --------------------------------------------------

// --------------------------------------------------
void Logger::crit(this Logger &self, const char *fmt, ...)
{
        va_list args;
        va_start(args, fmt);
        self.va_log(LogLevel::Crit, fmt, args);
        va_end(args);
}
// --------------------------------------------------

} /* namespace kernel::lib::log */