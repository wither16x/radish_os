#pragma once

#include <proc/pid.hpp>

namespace Kiwi::Proc
{
        int kill(pid_t pid);
} // namespace Kiwi::Proc