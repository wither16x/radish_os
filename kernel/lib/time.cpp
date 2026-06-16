#include <drivers/pit.hpp>
#include <lib/time.hpp>

namespace kernel::lib {

void sleep(u64 ms)
{
        drivers::pit::sleep(ms);
}

} /* namespace kernel::lib */