#include <drivers/pit.hpp>
#include <lib/time.hpp>

namespace Kiwi::Lib
{
        void sleep(u64 ms)
        {
                Drivers::Pit::sleep(ms);
        }

        u64 getCurrentTime()
        {
                return Drivers::Pit::getSeconds() + Drivers::Pit::getTics();
        }
} // namespace Kiwi::Lib