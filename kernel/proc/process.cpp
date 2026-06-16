#include <lib/time.hpp>
#include <proc/process.hpp>
#include <panic.hpp>

using kernel::lib::Timer;

namespace kernel::proc {

void proc_init(Process *p, int id, void (*entry)(), void (*exit)())
{
        p->entry        = entry;
        p->exit         = exit;

        p->id           = id;

        p->state        = ProcessState::Idle;
        p->timer        = Timer(0, TIME_PER_PROCESS, 1);
}

} /* namespace kernel::proc */