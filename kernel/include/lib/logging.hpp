#pragma once

#include <lib/typing.hpp>
#include <lib/string.hpp>
#include <lib/print.hpp>

namespace Kiwi::Lib
{
        /// A logger is used to output informations at different
        /// levels of importance.
        class Logger
        {
                static constexpr int MAX_CONTEXT_BYTES = 50;

                char context[MAX_CONTEXT_BYTES];

                static constexpr const char *log_level_str[6] = {
                        "\033[37mdebug",
                        "\033[36minfo",
                        "\033[32mok",
                        "\033[33mwarn",
                        "\033[31merr",
                        "\033[35mcrit"
                };

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

                void setContext(this Logger &self, const String<Logger::MAX_CONTEXT_BYTES> &ctx);

                template<usize N = 255, typename... ARGS>
                void log(this Logger &self, LogLevel lv, const type_identity_t<String<N>> &fmt, ARGS &&...args)
                {
                        String<N> msg = formatString<N>(fmt, static_cast<ARGS &&>(args)...);

                        println<N>("[{}] {}\033[37m: {}",
                                static_cast<const char *>(self.context),
                                self.log_level_str[toUnderlying(lv)],
                                msg
                        );
                }

                template<usize N = 255, typename... ARGS>
                void debug(this Logger &self, const type_identity_t<String<N>> &fmt, ARGS &&...args)
                {
                        self.log(LogLevel::Debug, fmt, (args)...);
                }

                template<usize N = 255, typename... ARGS>
                void info(this Logger &self, const type_identity_t<String<N>> &fmt, ARGS &&...args)
                {
                        self.log(LogLevel::Info, fmt, (args)...);
                }

                template<usize N = 255, typename... ARGS>
                void ok(this Logger &self, const type_identity_t<String<N>> &fmt, ARGS &&...args)
                {
                        self.log(LogLevel::Ok, fmt, (args)...);
                }

                template<usize N = 255, typename... ARGS>
                void warn(this Logger &self, const type_identity_t<String<N>> &fmt, ARGS &&...args)
                {
                        self.log(LogLevel::Warn, fmt, (args)...);
                }

                template<usize N = 255, typename... ARGS>
                void err(this Logger &self, const type_identity_t<String<N>> &fmt, ARGS &&...args)
                {
                        self.log(LogLevel::Err, fmt, (args)...);
                }

                template<usize N = 255, typename... ARGS>
                void crit(this Logger &self, const type_identity_t<String<N>> &fmt, ARGS &&...args)
                {
                        self.log(LogLevel::Crit, fmt, (args)...);
                }
        };
} // namespace Kiwi::Lib