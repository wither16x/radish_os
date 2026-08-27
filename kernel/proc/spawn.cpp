#include <proc/spawn.hpp>
#include <proc/process.hpp>
#include <proc/elf.hpp>
#include <proc/scheduler.hpp>
#include <proc/pid.hpp>
#include <kernel.hpp>
#include <lib/string.hpp>
#include <lib/typing.hpp>
#include <lib/logging.hpp>
#include <cpu/cpu.hpp>
#include <cpu/userspace.hpp>
#include <panic.hpp>

namespace Kiwi::Proc
{
        namespace
        {
                Process *loadProgramAsProcess(const Lib::String &path)
                {
                        // create the process' pml4t
                        Mem::PML4T &kpml4t = getKernelPml4t();
                        Mem::PML4T proc_pml4t;
                        proc_pml4t.init(kpml4t);

                        // load the file (assume ELF64)
                        Elf::ElfInfo elf_info;
                        int load_res = Elf::loadElf(&proc_pml4t, path, &elf_info);
                        if (load_res != 0)
                                return nullptr; // could not load executable

                        elf_info.entry = reinterpret_cast<Elf::elf_entry_t>(elf_info.address);

                        // create the process
                        Process *proc = new Process(allocatePid(), &elf_info, proc_pml4t);

                        return proc;
                }
        } // anonymous namespace

        int spawn(const Lib::String &path)
        {
                Cpu::disableInterrupts();

                Process *proc = loadProgramAsProcess(path);
                if (not proc) {
                        Lib::Log::logger.err("failed to spawn process: failed to load %s", path.raw());
                        return -1;
                }

                // tell the scheduler that the process exists
                Scheduler::addProcess(proc);

                Scheduler::setCurrentProcess(proc);
                Process *p = Scheduler::getCurrentProcess();
                p->loadPml4t();

                if (not p->getEntry())
                        panic("failed to spawn process: null entry point");

                Cpu::enter_userspace(
                        reinterpret_cast<void *>(p->getStackFrame()->rip),
                        reinterpret_cast<void *>(p->getStackFrame()->rsp)
                );

                Cpu::enableInterrupts();

                return 0;
        }
} // namespace Kiwi::Proc