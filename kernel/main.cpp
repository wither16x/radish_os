#include <boot/bootinfo.hpp>
#include <boot/limine.hpp>
#include <cpu/assembly.hpp>
#include <cpu/gdt.hpp>
#include <cpu/idt.hpp>
#include <drivers/serial.hpp>
#include <lib/alloc.hpp>
#include <lib/status.hpp>
#include <lib/typing.hpp>
#include <mem/heap.hpp>
#include <mem/pmm.hpp>
#include <mem/vmm.hpp>
#include <panic.hpp>

using namespace kernel;

using lib::u64, lib::uptr;
using lib::Status;

namespace {

[[gnu::used, gnu::section(".limine_requests")]]
volatile boot::limine::Revision limine_base_revision = boot::limine::base_revision(6);

} /* anonymous namespace */

namespace {

[[gnu::used, gnu::section(".limine_requests_start")]]
volatile boot::limine::StartMarker limine_requests_start_marker = boot::limine::requests_start_marker();

[[gnu::used, gnu::section(".limine_requests_end")]]
volatile boot::limine::EndMarker limine_requests_end_marker = boot::limine::requests_end_marker();

} /* anonymous namespace */

extern void (*__init_array[])();
extern void (*__init_array_end[])();

extern "C" void kernel_main()
{
        if (!limine_base_revision.is_supported())
                panic("limine base revision not supported"); // unprintable message

        for (u64 i = 0; &__init_array[i] != __init_array_end; i++)
                __init_array[i]();

        if (drivers::serial::init_port(drivers::serial::Port::COM1) != Status::Ok)
                panic("no display device"); // so the message cannot be printed lol

        cpu::GDT gdt;
        gdt.load();

        cpu::IDT idt;
        idt.load();

        boot::BootInfo bootinfo;

        mem::pmm.init(bootinfo.memmap);

        mem::vmm.init(bootinfo.hhdm.offset, bootinfo.executable, bootinfo.memmap);
        mem::vmm.load();

        mem::kheap.init();

        panic("nothing to do");
}