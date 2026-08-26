#include <cpu/sse2.hpp>

namespace Kiwi::Cpu
{
        namespace
        {
                extern "C" void __enable_sse2();
        } // anonymous namespace

        void enableSse2()
        {
                __enable_sse2();
        }
} // namespace Kiwi::Cpu