#include <panic/unwind.hpp>
#include <lib/typing.hpp>
#include <lib/print.hpp>
#include <lib/memory.hpp>

#include <kernel_sym_entries.h>

namespace Kiwi::Panic
{
        const char *lookupSymbol(Lib::uptr addr)
        {
                const char *best_match = "???";
                Lib::uptr best_addr = 0;

                for (Lib::usize i = 0; i < kernel_symbol_count; i++) {
                        if (kernel_symbols[i].addr <= addr and kernel_symbols[i].addr >= best_addr) {
                                best_addr = kernel_symbols[i].addr;
                                best_match = kernel_symbols[i].name;
                        }
                }

                return best_match;
        }

	void captureStackTrace(void **buffer, Lib::u32 max_frames)
	{
		if (max_frames == 0 or not buffer)
			return;

		StackFrame *frame;
		__asm__ volatile ("movq %%rbp, %0" : "=r"(frame));

		for (Lib::u32 i = 0; i < max_frames; i++) {
			if (not frame or not frame->rbp)
				break;

			buffer[i] = frame->return_address;
			frame = frame->rbp;
		}
	}

	void dumpStackTrace()
	{
		void *trace[STACK_TRACE_DEPTH];
		Lib::memset(trace, 0, sizeof(trace));

		captureStackTrace(trace, STACK_TRACE_DEPTH);

                for (Lib::u8 i = 0; i < STACK_TRACE_DEPTH; i++) {
                        void *addr = trace[i];
                        if (not addr)
                                break;

                        const char *name = lookupSymbol(reinterpret_cast<Lib::uptr>(addr));
                        Lib::println<255>("[trace frame 0x{}] 0x{}: {}", Lib::hex(i), Lib::hex(reinterpret_cast<Lib::uptr>(addr)), name);
                }
	}
} // namespace Kiwi::Panic