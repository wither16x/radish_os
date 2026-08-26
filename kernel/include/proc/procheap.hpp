#pragma once

#include <lib/typing.hpp>
#include <mem/pml4t.hpp>

namespace Kiwi::Proc
{
        /// Process heap pages are linear from `start` to `limit`.
        /// `last_page` represents the last mapped page. It changes when
        /// the heap gets smaller or bigger.
        class ProcessHeap
        {
                Lib::uptr start;
                Lib::uptr last_page;
                Lib::uptr limit;
                Mem::PML4T &pml4t;

        public:
                ProcessHeap(Lib::uptr start, Lib::uptr last_page, Lib::uptr limit, Mem::PML4T &pml4t);

                bool extend(this ProcessHeap &self, int pages);
                bool shorten(this ProcessHeap &self, int pages);
                
                void reset(this ProcessHeap &self, Lib::uptr new_start, Mem::PML4T &pml4t);

                Lib::uptr getStart(this const ProcessHeap &self);
                Lib::uptr getLastPage(this const ProcessHeap &self);
                Lib::uptr getLimit(this const ProcessHeap &self);
        };
} // namespace Kiwi::Proc