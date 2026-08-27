#include <proc/pid.hpp>

namespace Kiwi::Proc
{
        namespace
        {
                pid_t current_pid = 1;
        } // anonymous namespace

        pid_t allocatePid()
        {
                return current_pid++;
        }
} // namespace Kiwi::Proc