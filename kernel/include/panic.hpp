#pragma once

#include <lib/string.hpp>
#include <lib/typing.hpp>
#include <lib/logging.hpp>
#include <cpu/cpu.hpp>
#include <proc/scheduler.hpp>

namespace Kiwi
{
        constexpr Lib::usize PANIC_MESSAGE_SIZE = 2048;

        void *getReturnAddress(int i);
        const char *lookupSymbol(Lib::uptr addr);
        void dumpStackTrace(Lib::u8 depth);

        /// This function should be called only if an unrecoverable error happened.
        /// If there is a process running, then this process is immediately aborted.
        /// Otherwise, the interrupts are canceled and the CPU is idling forever.
        template<typename... ARGS>
        void panic(Lib::String<PANIC_MESSAGE_SIZE> fmt, ARGS &&...args)
        {
                Cpu::disableInterrupts();
                Proc::Scheduler::lock();

                kcontext.logger.setContext("panic handler");

                Lib::println("\r\n==================== KERNEL PANIC ====================");
                Lib::String<PANIC_MESSAGE_SIZE> msg = formatString<PANIC_MESSAGE_SIZE>(fmt, static_cast<ARGS &&>(args)...);
                println<PANIC_MESSAGE_SIZE>("{}", msg);

                Lib::println("Stack trace:");
                dumpStackTrace(15);

                Proc::Process *curr_proc = Proc::Scheduler::getCurrentProcess();
                if (not curr_proc) {
                        kcontext.logger.info("idling");
                        while (true) {
                                Cpu::disableInterrupts();
                                Cpu::idle();
                        }
                }

                curr_proc->die();
                kcontext.logger.ok("process with ID {} aborted", curr_proc->getId());

                Proc::Scheduler::unlock();
                Cpu::enableInterrupts();
        }

        template<>
        inline void panic(Lib::String<PANIC_MESSAGE_SIZE> fmt)
        {
                Cpu::disableInterrupts();
                Proc::Scheduler::lock();

                kcontext.logger.setContext("panic handler");

                Lib::println("\r\n==================== KERNEL PANIC ====================");
                println<PANIC_MESSAGE_SIZE>("{}", fmt);

                Lib::println("Stack trace:");
                dumpStackTrace(15);

                Proc::Process *curr_proc = Proc::Scheduler::getCurrentProcess();
                if (not curr_proc) {
                        kcontext.logger.info("idling");
                        while (true) {
                                Cpu::disableInterrupts();
                                Cpu::idle();
                        }
                }

                curr_proc->die();
                kcontext.logger.ok("process with ID {} aborted", curr_proc->getId());

                Proc::Scheduler::unlock();
                Cpu::enableInterrupts();
        }
} // namespace Kiwi