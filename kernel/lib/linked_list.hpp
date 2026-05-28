#pragma once

#include <lib/typing.hpp>

namespace kernel::lib {

struct LinkedListHeader {
        LinkedListHeader *next;
        LinkedListHeader *prev;
};

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

        HDR *first(this LinkedList<HDR> &self)
        {
                return self.hdr;
        }

        HDR *last(this LinkedList<HDR> &self)
        {
                HDR *curr = self.hdr;

                while (curr->next)
                        curr = static_cast<HDR *>(curr->next);

                return curr;
        }

        void reset(this LinkedList<HDR> &self)
        {
                self.hdr = nullptr;
        }

        void set_base(this LinkedList<HDR> &self, uptr base)
        {
                self.hdr = reinterpret_cast<HDR *>(base);
        }

        void append(this LinkedList<HDR> &self, HDR *node)
        {
                HDR *last = self.last();
                last->next = node;

                node->prev = last;
                node->next = nullptr;
        }
};

} /* namespace kernel::lib */