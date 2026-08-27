#include "cpu/cpu.hpp"
#include <kernel.hpp>
#include <panic.hpp>
#include <boot/bootinfo.hpp>
#include <boot/limine.hpp>
#include <cpu/assembly.hpp>
#include <cpu/gdt.hpp>
#include <cpu/idt.hpp>
#include <cpu/sse2.hpp>
#include <drivers/console.hpp>
#include <drivers/framebuffer.hpp>
#include <drivers/keyboard.hpp>
#include <drivers/pic.hpp>
#include <drivers/pit.hpp>
#include <drivers/ps2kbd.hpp>
#include <drivers/serial.hpp>
#include <fs/devfs.hpp>
#include <fs/ustar.hpp>
#include <fs/vfs.hpp>
#include <lib/filesystem.hpp>
#include <lib/logging.hpp>
#include <lib/memory.hpp>
#include <lib/typing.hpp>
#include <mem/heap.hpp>
#include <mem/pmm.hpp>
#include <mem/vmm.hpp>
#include <proc/spawn.hpp>
#include <proc/scheduler.hpp>
#include <test.hpp>

extern void (*__init_array[])();
extern void (*__init_array_end[])();

namespace Kiwi
{
        /// Call the global constructors to initialize them.
        /// A heap allocator is required.
        void callGlobalConstructors()
        {
                for (Lib::u64 i = 0; &__init_array[i] != __init_array_end; i++) {
                        __init_array[i]();
                        Lib::Log::logger.ok("initialized global constructor %u", i);
                }

                Lib::Log::logger.ok("called global constructors");
        }

        /// Mount the initrd.
        void mountInitrd(Boot::BootInfo::ModuleInfo &info)
        {
                // 'I' for initrd
                Lib::u64 idx = Boot::BootInfo::ModuleInfo::MAX_MODULES + 1;
                for (Lib::u64 i = 0; i < info.count; i++) {
                        if (Kiwi::Lib::strcmp(info.modules[i].path, "/initrd.tar") == 0)
                                idx = i;
                }
                if (idx == Boot::BootInfo::ModuleInfo::MAX_MODULES + 1)
                        Lib::Log::logger.err("initrd not found");
                else
                        Lib::Log::logger.ok("found initrd");

                Fs::Vfs::mount('I', new Fs::Ustar::USTAR(info.modules[idx].address));

                Lib::Log::logger.ok("mounted initrd as I");
        }

        void mountDevices()
        {
                Fs::Vfs::mount('D', new Fs::Devfs::Devfs());
                Lib::Log::logger.ok("mounted devfs as D");
        }

        /// Unmount the initrd (do it at the end).
        void unmountInitrd()
        {
                Fs::Vfs::unmount('I');
                Lib::Log::logger.ok("unmounted initrd");
        }

        /// Set up the kernel console.
        void initConsole()
        {
                Drivers::Console::Console kconsole(Drivers::Framebuffer::getWidth(), Drivers::Framebuffer::getHeight());
                Drivers::Console::setConsole(kconsole);
                Drivers::Console::Console &console = Drivers::Console::getConsole();
                console.initFont("I:/fonts/zap-light20.psf");

                Lib::Log::logger.ok("initialized console");
                Lib::Log::logger.info("framebuffer should now be used for display");
        }

        /// Idle.
        void kernelHang()
        {
                panic("nothing to do");
        }

        /// Kernel entry point.
        extern "C" void kernel_main()
        {
                if (not Boot::Limine::getBaseRevision().isSupported())
                        panic("limine base revsion not supported"); // you wont see the message

                if (not Drivers::Serial::initPort(Drivers::Serial::Port::SERIAL_COM1))
                        panic("no display device"); // so the message cannot be printed lol

                Lib::Log::logger.setContext("kernel");

                Cpu::Gdt gdt;
                setKernelGdt(gdt);
                getKernelGdt().init();
                getKernelGdt().load();

                Cpu::Idt idt;
                setKernelIdt(idt);
                getKernelIdt().init();
                getKernelIdt().load();

                getKernelGdt().getTss().flush();

                Boot::BootInfo bootinfo;
                setKernelHhdmOffset(bootinfo.hhdm.offset);

                Mem::Pmm::init(bootinfo.memmap);

                Mem::PML4T kpml4t = Mem::Vmm::init(bootinfo.hhdm.offset, bootinfo.executable, bootinfo.memmap);
                kpml4t.load();
                setKernelPml4t(kpml4t);

                Mem::Heap::init();

                Drivers::Pic::remap();
                Lib::Log::logger.ok("remapped 8259 pic");
                Drivers::Pic::irqMaskAll();
                Lib::Log::logger.ok("masked all irq");

                Drivers::Pit::init();

                Drivers::Ps2Kbd::init();

                Cpu::enableInterrupts();

                callGlobalConstructors();

                mountDevices();

                Drivers::Keyboard::init();

                mountInitrd(bootinfo.modules);

                Drivers::Framebuffer::init(
                        bootinfo.framebuffer.address,
                        bootinfo.framebuffer.width,
                        bootinfo.framebuffer.height,
                        bootinfo.framebuffer.pitch
                );
                Lib::Log::logger.ok("initialized framebuffer");

                Proc::Scheduler::init();
                Cpu::enableSse2();
                Lib::Log::logger.ok("enabled sse2");

                initConsole();

                Test::testLib();
                Test::testFloat();

                Proc::spawn("I:/bin/init");
                
                unmountInitrd();
                Fs::Vfs::unmount('D');

                kernelHang();
        }
} // namespace kernel