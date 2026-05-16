#include <boot/bootinfo.hpp>
#include <boot/limine.hpp>
#include <cpu/assembly.hpp>
#include <cpu/gdt.hpp>
#include <cpu/idt.hpp>
#include <drivers/serial.hpp>
#include <lib/print.hpp>
#include <lib/status.hpp>
#include <lib/typing.hpp>
#include <panic.hpp>

using namespace kernel;

namespace {

[[gnu::used, gnu::section(".limine_requests")]]
volatile boot::limine::revision limine_base_revision = boot::limine::base_revision(6);

}

namespace {

[[gnu::used, gnu::section(".limine_requests_start")]]
volatile boot::limine::start_marker limine_requests_start_marker = boot::limine::requests_start_marker();

[[gnu::used, gnu::section(".limine_requests_end")]]
volatile boot::limine::end_marker limine_requests_end_marker = boot::limine::requests_end_marker();

}

extern void (*__init_array[])();
extern void (*__init_array_end[])();

extern "C" void kernel_main()
{
        if (!limine_base_revision.is_supported())
                panic("limine base revision not supported"); // unprintable message

        for (lib::u64 i = 0; &__init_array[i] != __init_array_end; i++)
                __init_array[i]();

        if (drivers::serial::init_port(drivers::serial::Port::COM1) != lib::Status::Ok)
                panic("no display device"); // so the message cannot be printed lol

        cpu::GDT gdt;
        gdt.load();

        cpu::IDT idt;
        idt.load();

        boot::boot_info bootinfo;
        lib::println("Recovered boot info:");
        lib::println("* Bootloader: %s version %s", bootinfo.bootloader.name, bootinfo.bootloader.version);
        lib::println("* Firmware: %s", bootinfo.firmware_type.str);

        panic("nothing to do");
}