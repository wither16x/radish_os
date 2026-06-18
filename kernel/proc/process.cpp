#include "lib/logging.hpp"
#include <lib/typing.hpp>
#include <proc/process.hpp>
#include <panic.hpp>

using kernel::lib::u8, kernel::lib::u64, kernel::lib::usize;

namespace kernel::proc {

void proc_init(Process *p, int id, void (*entry)())
{
        p->entry        = entry;
        p->id           = id;
	p->time		= 0;
}

extern "C" void proc_exec(Process *p)
{
        lib::log::logger.debug("executing proc");
        p->entry();
}

} /* namespace kernel::proc */