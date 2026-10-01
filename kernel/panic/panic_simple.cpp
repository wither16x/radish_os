#include <panic/panic.hpp>
#include <panic/panic_simple.hpp>

namespace Kiwi::Panic
{
        void panic_simple(const char *msg)
        {
                panic(Lib::String<PANIC_MESSAGE_SIZE>(msg));
        }
} // namespace Kiwi::Panic