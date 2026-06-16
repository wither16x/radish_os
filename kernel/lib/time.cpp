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

Timer::Timer(u64 start, u64 limit, u64 step)
{
        this->time      = start;
        this->limit     = limit;
        this->step      = step;
}

bool Timer::clock(this Timer &self)
{
        if (self.time >= self.limit)
                return false;

        if (drivers::pit::consumed_tick()) {
                self.time += self.step;
                return true;
        }

        return true;    // yes, even though the time did not change
}

void Timer::reset(this Timer &self, u64 start, u64 limit)
{
        self.time = start;
        self.limit = limit;
}

u64 Timer::get_time(this const Timer &self)
{
        return self.time;
}

} /* namespace kernel::lib */