#include <boot/bootinfo.hpp>
#include <boot/limine.hpp>
#include <cpu/gdt.hpp>
#include <cpu/idt.hpp>
#include <drivers/serial.hpp>
#include <fs/tmpfs.hpp>
#include <fs/vfs.hpp>
#include <lib/logging.hpp>
#include <lib/status.hpp>
#include <lib/typing.hpp>
#include <mem/heap.hpp>
#include <mem/pmm.hpp>
#include <mem/vmm.hpp>
#include <panic.hpp>

using kernel::drivers::serial::init_port, kernel::drivers::serial::Port;
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
using namespace kernel::fs;

namespace {

[[gnu::used, gnu::section(".limine_requests")]]
volatile Revision limine_base_revision = base_revision(6);

} /* anonymous namespace */

namespace {

[[gnu::used, gnu::section(".limine_requests_start")]]
volatile StartMarker limine_requests_start_marker = requests_start_marker();

[[gnu::used, gnu::section(".limine_requests_end")]]
volatile EndMarker limine_requests_end_marker = requests_end_marker();

} /* anonymous namespace */

extern void (*__init_array[])();
extern void (*__init_array_end[])();

extern "C" void kernel_main()
{
        if (!limine_base_revision.is_supported())
                panic("limine base revision not supported"); // unprintable message

        if (init_port(Port::COM1) != Status::Ok)
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

        for (u64 i = 0; &__init_array[i] != __init_array_end; i++) {
                logger.info("initializing global constructor %u...", i);
                __init_array[i]();
        }

        vfs::mount('A', new tmpfs::TMPFS());

        vfs::create_dir("A:/bin");
        vfs::create_file("A:/README.txt");
        vfs::create_file("A:/hello.txt");
        vfs::create_file("A:/bin/executable");

        logger.debug("A:/ content:");
        
        kernel::lib::Vector<vfs::DirEntry> root;
        kernel::lib::Vector<vfs::DirEntry> bin;

        vfs::readdir("A:/", root);
        for (auto &nd : root)
                logger.debug("* %s", nd.name.raw());

        vfs::readdir("A:/bin", bin);
        logger.debug("A:/bin content: ");
        for (auto &nd : bin)
                logger.debug("* %s", nd.name.raw());
        
        vfs::write_file("A:/hello.txt", "Hello, world!");
        char buf[255];
        vfs::read_file("A:/hello.txt", buf, sizeof(buf));
        logger.debug("reading from A:/hello.txt: %s", buf);

        vfs::unmount('A');

        panic("nothing to do");
}