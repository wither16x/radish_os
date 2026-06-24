#include <kernel.hpp>
#include <boot/bootinfo.hpp>
#include <boot/limine.hpp>
#include <cpu/assembly.hpp>
#include <cpu/gdt.hpp>
#include <cpu/idt.hpp>
#include <drivers/console.hpp>
#include <drivers/framebuffer.hpp>
#include <drivers/pic.hpp>
#include <drivers/pit.hpp>
#include <drivers/serial.hpp>
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
#include <proc/process.hpp>
#include <proc/scheduler.hpp>

using namespace kernel::drivers;
using namespace kernel::fs;
using namespace kernel::mem;
using namespace kernel::proc;

using kernel::lib::u64, kernel::lib::uptr;
using kernel::lib::Status;
using kernel::lib::log::logger;
using kernel::panic;
using kernel::boot::limine::Revision, kernel::boot::limine::base_revision;
using kernel::boot::limine::StartMarker, kernel::boot::limine::requests_start_marker;
using kernel::boot::limine::EndMarker, kernel::boot::limine::requests_end_marker;
using kernel::boot::BootInfo;
using kernel::cpu::GDT, kernel::cpu::IDT;
using kernel::set_kernel_pml4t;

namespace {

[[gnu::used, gnu::section(".limine_requests")]]
volatile Revision limine_base_revision = base_revision(6);

[[gnu::used, gnu::section(".limine_requests_start")]]
volatile StartMarker limine_requests_start_marker = requests_start_marker();

[[gnu::used, gnu::section(".limine_requests_end")]]
volatile EndMarker limine_requests_end_marker = requests_end_marker();

} /* anonymous namespace */

extern void (*__init_array[])();
extern void (*__init_array_end[])();

extern char __proc_test_start[];
extern char __proc_test_end[];

void call_global_constructors()
{
        for (u64 i = 0; &__init_array[i] != __init_array_end; i++) {
                __init_array[i]();
                logger.ok("initialized global constructor %u", i);
        }

        logger.ok("called global constructors");
}

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

        logger.ok("mounted initrd");
}

void unmount_initrd()
{
        vfs::unmount('I');
        logger.ok("unmounted initrd");
}

void init_console()
{
        console::Console kconsole(framebuffer::get_width(), framebuffer::get_height());
        console::set_console(kconsole);
        console::Console &console = console::get_console();
        console.init_font("I:/fonts/zap-light20.psf");

        logger.ok("initialized console");
        logger.info("framebuffer should now be used for display");
}

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

void kernel_hang()
{
        panic("nothing to do");
}

extern "C" void kernel_main()
{
        if (!limine_base_revision.is_supported())
                panic("limine base revision not supported"); // unprintable message

        if (serial::init_port(serial::Port::COM1) != Status::Ok)
                panic("no display device"); // so the message cannot be printed lol

        logger.set_context("kernel");

        GDT gdt;
        gdt.load();

        IDT idt;
        idt.load();

        BootInfo bootinfo;

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
        logger.ok("initialized pit");

        pic::irq_unmask(0);
        logger.ok("unmasked irq 0: timer");

        kernel::cpu::sti();

        call_global_constructors();

        mount_initrd(bootinfo.modules);

        framebuffer::init(
                bootinfo.framebuffer.address,
                bootinfo.framebuffer.width,
                bootinfo.framebuffer.height,
                bootinfo.framebuffer.pitch
        );
        logger.ok("initialized framebuffer");

        init_console();

        __test_ls("I:/");

        u64 *test_proc_pml4t = vmm::create_pml4t(kernel::get_kernel_pml4t());
        uptr proc_addr = 0;
        int res = elf::load_elf(test_proc_pml4t, "I:/bin/test", bootinfo.hhdm.offset, &proc_addr);
        if (res == -1)
                logger.err("failed to load elf");

        void (*test_entry)() = reinterpret_cast<void (*)()>(proc_addr);
        Process test_proc(allocate_pid(), test_entry, test_proc_pml4t);
        scheduler::add_process(&test_proc);

        scheduler::init();

        // Snippet to execute a process `p`.
        // This only needs to be done once.
        // --------------------------------------
        scheduler::set_current_process(&test_proc);
        Process *p = scheduler::get_current_process();
        p->load();
        __asm__ volatile (
                "mov %0, %%rsp\n"
                "iretq\n"
                :
                : "r"(p->rsp)
        );

        unmount_initrd();

        kernel_hang();
}