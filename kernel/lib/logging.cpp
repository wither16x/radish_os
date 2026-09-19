#include <lib/memory.hpp>
#include <lib/args.hpp>
#include <lib/logging.hpp>
#include <lib/print.hpp>

namespace Kiwi::Lib::Log
{
        void Logger::setContext(this Logger &self, const String<Logger::MAX_CONTEXT_BYTES> &ctx)
        {
                usize i = 0;
                for (; ctx.raw()[i] and i < MAX_CONTEXT_BYTES - 1; i++)
                        self.context[i] = ctx.raw()[i];
                self.context[i] = '\0';
        }
} // namespace Kiwi::Lib::Log