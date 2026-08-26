#include <lib/print.hpp>
#include <cpu/io.hpp>
#include <cpu/irq.hpp>
#include <drivers/keyboard.hpp>
#include <drivers/pic.hpp>
#include <drivers/pit.hpp>
#include <kernel.hpp>
#include <lib/logging.hpp>
#include <lib/typing.hpp>
#include <proc/process.hpp>
#include <proc/scheduler.hpp>

namespace Kiwi::Cpu
{
        namespace
        {
                constexpr int MAX_IRQ_HANDLERS = 16;

                Lib::callable<void, IrqFrame &> handlers[MAX_IRQ_HANDLERS];
        } // anonymous namespace

        void register_irq(int n, Lib::callable<void, IrqFrame &> handler)
        {
                handlers[n] = handler;
                Drivers::Pic::irqUnmask(n);
        }

        extern "C" void irq_handler(IrqFrame &f)
        {
                if (f.irqno >= MAX_IRQ_HANDLERS) {
                        Lib::Log::logger.err("received unexpected irq: %u", f.irqno);
                        return;
                }

                void (*handler)(IrqFrame &) = handlers[f.irqno];
                if (not handler) {
                        Lib::Log::logger.err("no handler available for irq %u", f.irqno);
                        return;
                }
                
                handler(f);
        }
} // namespace Kiwi::Cpu