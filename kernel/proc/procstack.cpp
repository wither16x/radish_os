#include <proc/procstack.hpp>
#include <lib/typing.hpp>
#include <mem/pmm.hpp>
#include <mem/page.hpp>
#include <kernel.hpp>

namespace Kiwi::Proc
{
        ProcessKernelStack::ProcessKernelStack()
        {
                Lib::uptr hhdm_offset = getKernelHhdmOffset();

                this->frame = Mem::Pmm::allocateFrame();
                this->top = this->frame + hhdm_offset + Mem::PAGE_SIZE;
                this->pointer = reinterpret_cast<Lib::uptr *>(this->top);
        }

        void ProcessKernelStack::push(this ProcessKernelStack &self, Lib::uptr value)
        {
                if (not self.pointer)
                        return;

                *(--self.pointer) = value;
        }

        Lib::uptr ProcessKernelStack::get(this const ProcessKernelStack &self)
        {
                return reinterpret_cast<Lib::uptr>(self.pointer);
        }

        Lib::uptr ProcessKernelStack::getTop(this const ProcessKernelStack &self)
        {
                return self.top;
        }

        Lib::uptr ProcessKernelStack::getFrame(this const ProcessKernelStack &self)
        {
                return self.frame;
        }

        Lib::uptr *ProcessKernelStack::address(this ProcessKernelStack &self)
        {
                return reinterpret_cast<Lib::uptr *>(&self.pointer);
        }

        const Lib::uptr *ProcessKernelStack::address(this const ProcessKernelStack &self)
        {
                return reinterpret_cast<Lib::uptr *>(self.pointer);
        }
} // namespace Kiwi::Proc