#include <proc/procstack.hpp>
#include <lib/typing.hpp>
#include <mem/pmm.hpp>
#include <mem/page.hpp>
#include <kernel.hpp>

namespace kernel::proc {

ProcessKernelStack::ProcessKernelStack()
{
        lib::uptr hhdm_offset = get_kernel_hhdm_offset();

        this->frame = mem::pmm::allocate_frame();
        this->top = this->frame + hhdm_offset + mem::PAGE_SIZE;
        this->pointer = reinterpret_cast<lib::uptr *>(this->top);
}

void ProcessKernelStack::push(this ProcessKernelStack &self, lib::uptr value)
{
        if (not self.pointer)
                return;

        *(--self.pointer) = value;
}

lib::uptr ProcessKernelStack::get(this const ProcessKernelStack &self)
{
        return reinterpret_cast<lib::uptr>(self.pointer);
}

lib::uptr ProcessKernelStack::get_top(this const ProcessKernelStack &self)
{
        return self.top;
}

lib::uptr ProcessKernelStack::get_frame(this const ProcessKernelStack &self)
{
        return self.frame;
}

lib::uptr *ProcessKernelStack::address(this ProcessKernelStack &self)
{
        return reinterpret_cast<lib::uptr *>(&self.pointer);
}

const lib::uptr *ProcessKernelStack::address(this const ProcessKernelStack &self)
{
        return reinterpret_cast<lib::uptr *>(self.pointer);
}

} // namespace kernel::proc