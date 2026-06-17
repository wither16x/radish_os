#include <drivers/pit.hpp>
#include <lib/time.hpp>

namespace kernel::lib {

void sleep(u64 ms)
{
        drivers::pit::sleep(ms);
}

// I guess it works
u64 get_current_time()
{
        return drivers::pit::get_seconds() + drivers::pit::get_tics();
}

} /* namespace kernel::lib */