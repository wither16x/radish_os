#pragma once

#include <lib/typing.hpp>
#include <panic.hpp>

namespace kernel::lib {

/// Header of a block in a linked list. Every other header
/// used in a `LinkedList` must inherit from it.
struct LinkedListHeader {
        LinkedListHeader *next;
        LinkedListHeader *prev;
};

/// A list of blocks linked together.
template<typename HDR>
        requires __is_base_of(LinkedListHeader, HDR)
class LinkedList {
private:
        HDR *hdr;

public:
        LinkedList() = default;

        LinkedList(uptr base)
        {
                this->hdr = reinterpret_cast<HDR *>(base);
                this->hdr->next = nullptr;
                this->hdr->prev = nullptr;
        }

        /// Get the first block in the list.
        HDR *first(this LinkedList<HDR> &self)
        {
                return self.hdr;
        }

        /// Get the last block in the list.
        HDR *last(this LinkedList<HDR> &self)
        {
                HDR *curr = self.hdr;

                while (curr->next)
                        curr = static_cast<HDR *>(curr->next);

                return curr;
        }

        /// Reset the list.
        void reset(this LinkedList<HDR> &self)
        {
                self.hdr = nullptr;
        }

        /// Reset the first block in the list.
        void set_base(this LinkedList<HDR> &self, uptr base)
        {
                self.hdr = reinterpret_cast<HDR *>(base);
                self.hdr->next = nullptr;
                self.hdr->prev = nullptr;
        }

        /// Append a block to the list.
        void append(this LinkedList<HDR> &self, HDR *node)
        {
                HDR *last = self.last();
                last->next = node;

                node->prev = last;
                node->next = nullptr;
        }
};

} /* namespace kernel::lib */