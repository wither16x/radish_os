#pragma once

namespace kernel {

namespace cpu {

namespace assembly {

inline void hlt()
{
        __asm__ volatile ("hlt");
}

} /* namespace assembly */

} /* namespace cpu */

} /* namespace kernel */