#pragma once

#include <boot/bootinfo.hpp>
#include <lib/typing.hpp>

namespace kernel::mem {

class VMM {
public:
        void init(this VMM &self,
                lib::u64 hhdm,
                boot::BootInfo::ExecutableInfo &executable_info,
                boot::BootInfo::MemmapInfo &memmap_info
        );
        void load(this const VMM &self);

private:
        static constexpr lib::usize PageBytes = 0x1000;         // 4 KiB

        lib::u64 *pml4t;
        lib::u64 hhdm;
        boot::BootInfo::ExecutableInfo executable_info;
        boot::BootInfo::MemmapInfo memmap_info;

        lib::uptr get_pml4t();

        void map_page(this VMM &self, lib::uptr virt, lib::uptr phys, lib::u64 flags);

        void map_kernel(this VMM &self);
        void map_hhdm(this VMM &self);

        // Helpers
        // ----------------------------------------------------------------------------
        inline lib::uptr page_align_down(this const VMM &self, lib::uptr base)
        {
                return base / self.PageBytes * self.PageBytes;
        }

        inline lib::uptr page_align_up(this const VMM &self, lib::uptr base)
        {
                return (base + self.PageBytes - 1) / self.PageBytes * self.PageBytes;
        }

        inline lib::uptr page_div_up(this const VMM &self, lib::uptr base)
        {
                return (base + self.PageBytes - 1) / self.PageBytes;
        }
};

inline VMM vmm;

} /* namespace kernel::mem */