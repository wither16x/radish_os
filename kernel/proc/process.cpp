#include <proc/process.hpp>
#include <panic.hpp>

namespace kernel::proc {

void proc_init(Process *p, void (*entry)())
{
        p->entry = entry;
}

} /* namespace kernel::proc */