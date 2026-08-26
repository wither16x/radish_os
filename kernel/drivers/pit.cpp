#include <cpu/io.hpp>
#include <cpu/irq.hpp>
#include <drivers/pic.hpp>
#include <drivers/pit.hpp>
#include <lib/logging.hpp>
#include <lib/typing.hpp>
#include <proc/scheduler.hpp>

namespace Kiwi::Drivers::Pit
{
        namespace
        {
                /// Enumeration of the PIT ports.
                enum Port : Lib::u16
                {
                        PORT_COMMAND         = 0x43,
                        PORT_CHANNEL_0       = 0x40
                };

                Lib::u64 tics = 0;
                Lib::u64 seconds = 0;

                bool sleeping = false;

                void handleIrq(Cpu::IrqFrame &frame)
                {
                        (void)frame;

                        // increase time and consider that the interrupt is
                        // finished
                        tick();
                        Drivers::Pic::sendEoi(Drivers::Pic::IRQ_TIMER);

                        // schedule
                        if (not Proc::Scheduler::isActive())
                                return;
                        Proc::Scheduler::tick();
                }
        } // anonymous namespace

        void init()
        {
                Lib::u16 tps = 1000; // tics per second
                Lib::u16 divider = 1193181 / tps;

                Cpu::outputBytePort(Port::PORT_COMMAND, 0b00110100);
                Cpu::outputBytePort(Port::PORT_CHANNEL_0, divider & 0xff);
                Cpu::outputBytePort(Port::PORT_CHANNEL_0, (divider >> 8) & 0xff);

                Cpu::registerIrq(Drivers::Pic::IrqType::IRQ_TIMER, handleIrq);

                Lib::Log::logger.ok("initialized pit driver");
        }

        void tick()
        {
                tics++;

                // reset tics every second so the tic counter never
                // overflows
                // I will maybe add minutes, hours, days, and so on
                if (tics % 1000 == 0 and not sleeping) {
                        tics = 0;
                        seconds++;
                }
        }

        void sleep(Lib::u64 ms)
        {
                sleeping = true;
                while (tics < ms);
                sleeping = false;
        }

        Lib::u64 getTics()
        {
                return tics;
        }

        Lib::u64 getSeconds()
        {
                return seconds;
        }
} // namespace Kiwi::Drivers::Pit