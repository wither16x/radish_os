#include <proc/pid.hpp>

namespace kernel::proc {

namespace {

PID current_pid = 1;

} /* anonymous namespace */

PID allocate_pid()
{
        return current_pid++;
}

} /* namespace kernel::proc */