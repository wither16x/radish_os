#include <kernel.hpp>
#include <boot/bootinfo.hpp>
#include <boot/limine.hpp>
#include <cpu/assembly.hpp>
#include <cpu/gdt.hpp>
#include <cpu/idt.hpp>
#include <cpu/tss.hpp>
#include <drivers/console.hpp>
#include <drivers/framebuffer.hpp>
#include <drivers/pic.hpp>
#include <drivers/pit.hpp>
#include <drivers/keyboard.hpp>
#include <drivers/ps2kbd.hpp>
#include <drivers/serial.hpp>
#include <fs/devfs.hpp>
#include <fs/ustar.hpp>
#include <fs/vfs.hpp>
#include <lib/filesystem.hpp>
#include <lib/logging.hpp>
#include <lib/memory.hpp>
#include <lib/status.hpp>
#include <lib/typing.hpp>
#include <mem/heap.hpp>
#include <mem/pmm.hpp>
#include <mem/vmm.hpp>
#include <panic.hpp>
#include <proc/elf.hpp>
#include <proc/exec.hpp>
#include <proc/scheduler.hpp>

using namespace kernel::drivers;
using namespace kernel::fs;
using namespace kernel::mem;
using namespace kernel::proc;

using kernel::lib::u64, kernel::lib::uptr;
using kernel::lib::Status;
using kernel::lib::log::logger;
using kernel::boot::BootInfo;
using kernel::cpu::GDT, kernel::cpu::IDT, kernel::cpu::tss_flush, kernel::cpu::sti;

extern void (*__init_array[])();
extern void (*__init_array_end[])();

extern char __proc_test_start[];
extern char __proc_test_end[];

namespace kernel {

/// Call the global constructors to initialize them.
/// A heap allocator is required.
void call_global_constructors()
{
        for (u64 i = 0; &__init_array[i] != __init_array_end; i++) {
                __init_array[i]();
                logger.ok("initialized global constructor %u", i);
        }

        logger.ok("called global constructors");
}

/// Mount the initrd.
void mount_initrd(BootInfo::ModuleInfo &info)
{
        // 'I' for initrd
        u64 idx = BootInfo::ModuleInfo::MAX_MODULES + 1;
        for (u64 i = 0; i < info.count; i++) {
                if (kernel::lib::strcmp(info.modules[i].path, "/initrd.tar") == 0)
                        idx = i;
        }
        if (idx == BootInfo::ModuleInfo::MAX_MODULES + 1)
                logger.err("initrd not found");
        else
                logger.ok("found initrd");

        vfs::mount('I', new ustar::USTAR(info.modules[idx].address));

        logger.ok("mounted initrd as I");
}

void mount_devices()
{
        vfs::mount('D', new devfs::DEVFS());
        logger.ok("mounted devfs as D");
}

/// Unmount the initrd (do it at the end).
void unmount_initrd()
{
        vfs::unmount('I');
        logger.ok("unmounted initrd");
}

/// Setup the kernel console.
void init_console()
{
        console::Console kconsole(framebuffer::get_width(), framebuffer::get_height());
        console::set_console(kconsole);
        console::Console &console = console::get_console();
        console.init_font("I:/fonts/zap-light20.psf");

        logger.ok("initialized console");
        logger.info("framebuffer should now be used for display");
}

/// Test function to display files and directories in the
/// initrd.
void __test_ls(const kernel::lib::String &path)
{
        kernel::lib::usize count = 0;
        kernel::lib::getdirentn(path, &count);

        for (kernel::lib::usize i = 0; i < count; i++) {
                vfs::DirEntry entry;
                kernel::lib::getdirent(path, &entry, i);

                logger.info("* %s", entry.name.raw());;
                if (entry.is_dir)
                        __test_ls(path + entry.name);
        }
}

/// Idle.
void kernel_hang()
{
        panic("nothing to do");
}

/// Kernel entry point.
extern "C" void kernel_main()
{
        if (!boot::limine::get_base_revision().is_supported())
                panic("limine base revsion not supported"); // you wont see the message

        if (serial::init_port(serial::Port::SERIAL_COM1) != Status::Ok)
                panic("no display device"); // so the message cannot be printed lol

        logger.set_context("kernel");

        GDT gdt;
        gdt.load();

        IDT idt;
        idt.init();
        set_kernel_idt(idt);
        IDT &kidt = get_kernel_idt();
        kidt.load();

        tss_flush();

        BootInfo bootinfo;
        set_kernel_hhdm_offset(bootinfo.hhdm.offset);

        pmm::init_stage1(bootinfo.memmap);

        u64 *kpml4t = vmm::init(bootinfo.hhdm.offset, bootinfo.executable, bootinfo.memmap);
        vmm::load(kpml4t);
        set_kernel_pml4t(kpml4t);

        heap::init();

        pmm::init_stage2();

        pic::remap();
        logger.ok("remapped 8259 pic");
        pic::irq_mask_all();
        logger.ok("masked all irq");

        pit::init();

        ps2kbd::init();

        keyboard::init();

        sti();

        call_global_constructors();

        mount_devices();
        mount_initrd(bootinfo.modules);

        framebuffer::init(
                bootinfo.framebuffer.address,
                bootinfo.framebuffer.width,
                bootinfo.framebuffer.height,
                bootinfo.framebuffer.pitch
        );
        logger.ok("initialized framebuffer");

        init_console();

        logger.info("-------------------------------------");
        logger.info("list initrd content");
        logger.info("-------------------------------------");
        __test_ls("I:/");
        logger.info("-------------------------------------");
        logger.info("list of devices");
        logger.info("-------------------------------------");
        __test_ls("D:/");
        logger.info("-------------------------------------");

        scheduler::init();

        exec("I:/bin/hello");
        
        unmount_initrd();
        vfs::unmount('D');

        kernel_hang();
}

} /* namespace kernel */