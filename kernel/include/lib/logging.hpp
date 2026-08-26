#pragma once

#include <lib/args.hpp>

namespace Kiwi::Lib::Log
{
        /// A logger is used to output informations at different
        /// levels of importance.
        class Logger
        {
                static constexpr int MAX_CONTEXT_BYTES = 50;

                char context[MAX_CONTEXT_BYTES];

        public:
                /// Enumeration of log levels
                /// Debug                : information for the developer
                /// Info                 : information for everyone
                /// Ok                   : something has been done successfully
                /// Warn                 : some shit has been done but it is not an issue
                /// Err                  : some shit has been done and it is an issue
                /// Crit                 : something has been completely fucked up
                enum class LogLevel
                {
                        Debug,
                        Info,
                        Ok,
                        Warn,
                        Err,
                        Crit
                };

                /// Tell the logger who is logging.
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
} // namespace Kiwi::Lib::Log