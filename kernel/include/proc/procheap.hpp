#pragma once

#include <lib/typing.hpp>
#include <mem/pml4t.hpp>

namespace kernel::proc {

/// Process heap pages are linear from `start` to `limit`.
/// `last_page` represents the last mapped page. It changes when
/// the heap gets smaller or bigger.
class ProcessHeap {
        lib::uptr start;
        lib::uptr last_page;
        lib::uptr limit;
        mem::PML4T &pml4t;

public:
        ProcessHeap(lib::uptr start, lib::uptr last_page, lib::uptr limit, mem::PML4T &pml4t);

        bool extend(this ProcessHeap &self, int pages);
        bool shorten(this ProcessHeap &self, int pages);

        lib::uptr get_start(this const ProcessHeap &self);
        lib::uptr get_last_page(this const ProcessHeap &self);
        lib::uptr get_limit(this const ProcessHeap &self);
};

} // namespace kernel::proc