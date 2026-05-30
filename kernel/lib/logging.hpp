#pragma once

#include <lib/args.hpp>

namespace kernel::lib::log {

class Logger {
private:
        static constexpr int MaxContextBytes = 50;

        char context[MaxContextBytes];

public:
        // Debug                : information for the developer
        // Info                 : information for everyone
        // Ok                   : something has been done successfully
        // Warn                 : some shit has been done but it is not an issue
        // Err                  : some shit has been done and it is an issue
        // Crit                 : something has been completely fucked up
        enum class LogLevel : int {
                Debug,
                Info,
                Ok,
                Warn,
                Err,
                Crit
        };

        void set_context(this Logger &self, const char *ctx);

        void va_log(this Logger &self, LogLevel lv, const char *fmt, va_list args);
        void log(this Logger &self, LogLevel lv, const char *fmt, ...);
        // wrappers
        void debug(this Logger &self, const char *fmt, ...);
        void info(this Logger &self, const char *fmt, ...);
        void ok(this Logger &self, const char *fmt, ...);
        void warn(this Logger &self, const char *fmt, ...);
        void err(this Logger &self, const char *fmt, ...);
        void crit(this Logger &self, const char *fmt, ...);
};

inline Logger logger;

} /* namespace kernel::lib::log */