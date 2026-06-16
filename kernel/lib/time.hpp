#pragma once

#include <lib/typing.hpp>

namespace kernel::lib {

void sleep(u64 ms);
// return current time in miliseconds since PIT initialization
u64 get_current_time();

class Timer {
private:
        u64 time;
        u64 limit;
        u64 step;       // in ms

public:
        Timer() = default;
        Timer(u64 start, u64 limit, u64 step);

        // return if the clock worked or not
        bool clock(this Timer &self);
        void reset(this Timer &self, u64 start, u64 limit);

        u64 get_time(this const Timer &self);
};

} /* namespace kernel::lib */