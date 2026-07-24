#pragma once

#include <lib/typing.hpp>

namespace kernel::cpu {

/// Representation of a task state segment.
/// Note that we only use RSP0.
struct [[gnu::packed]] TSSData {
        lib::u32 __reserved0;
        lib::u64 rsp0;
        lib::u64 rsp1;
        lib::u64 rsp2;
        lib::u64 __reserved1;
        lib::u64 ist[7];
        lib::u64 __reserved2;
        lib::u64 __reserved3;
        lib::u16 iobp;
};

class TSS {
        TSSData data;

public:
        void init(this TSS &self, lib::uptr kernel_rsp);
        void flush() const;
        void reset_stack(this TSS &self, lib::uptr kernel_rsp);

        const TSSData &get_data(this const TSS &self);
};

} /* namespace kernel::cpu */