#include <boot/bootinfo.hpp>
#include <boot/limine.hpp>
#include <cpu/gdt.hpp>
#include <cpu/idt.hpp>
#include <drivers/console.hpp>
#include <drivers/framebuffer.hpp>
#include <drivers/serial.hpp>
#include <fs/ustar.hpp>
#include <fs/vfs.hpp>
#include <lib/logging.hpp>
#include <lib/memory.hpp>
#include <lib/status.hpp>
#include <lib/typing.hpp>
#include <mem/heap.hpp>
#include <mem/pmm.hpp>
#include <mem/vmm.hpp>
#include <panic.hpp>

using namespace kernel::drivers;
using namespace kernel::fs;

using kernel::lib::u64, kernel::lib::uptr;
using kernel::lib::Status;
using kernel::lib::log::logger;
using kernel::panic;
using kernel::boot::limine::Revision, kernel::boot::limine::base_revision;
using kernel::boot::limine::StartMarker, kernel::boot::limine::requests_start_marker;
using kernel::boot::limine::EndMarker, kernel::boot::limine::requests_end_marker;
using kernel::boot::BootInfo;
using kernel::cpu::GDT, kernel::cpu::IDT;
using kernel::mem::pmm, kernel::mem::vmm, kernel::mem::kheap;

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
        u64 idx = BootInfo::ModuleInfo::MaxModules + 1;
        for (u64 i = 0; i < info.count; i++) {
                if (kernel::lib::strcmp(info.modules[i].path, "/initrd.tar") == 0)
                        idx = i;
        }
        if (idx == BootInfo::ModuleInfo::MaxModules + 1)
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
        console::Console *console = console::get_console();
        console->init_font("I:/fonts/zap-light20.psf");

        logger.ok("initialized console");
        logger.info("framebuffer should now be used for display");
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

        pmm.init_stage1(bootinfo.memmap);

        vmm.init(bootinfo.hhdm.offset, bootinfo.executable, bootinfo.memmap);
        vmm.load();

        kheap.init();

        pmm.init_stage2();

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

        unmount_initrd();

        // idle
        panic("nothing to do");
}