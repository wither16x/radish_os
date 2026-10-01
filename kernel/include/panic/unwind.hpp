#pragma once

#include <lib/typing.hpp>

namespace Kiwi::Panic
{
	constexpr Lib::u8 STACK_TRACE_DEPTH = 15;

	struct StackFrame
	{
		struct StackFrame *rbp;
		void *return_address;
	};

        const char *lookupSymbol(Lib::uptr addr);
	void captureStackTrace(void **buffer, Lib::u32 max_frames);
	void dumpStackTrace();
} // namespace Kiwi::Panic