#pragma once

#include <lib/typing.hpp>

namespace kernel::mem {

class Heap {
public:
        struct BlockHeader {
                lib::usize bytes;
                bool free;
                BlockHeader *next;
                BlockHeader *prev;
        };

        void init(this Heap &self);

        // Allocate `n` bytes
        void *allocate(this Heap &self, lib::usize n);

private:
        BlockHeader *block_list;
        BlockHeader *curr_block;

        lib::usize pages;               // amount of allocated pages

        // Map new pages to handle more blocks
        void extend(this Heap &self);

        // This function may not split the block if it is not
        // necessary
        void split_block(this Heap &self, lib::usize super_size);

        // Find a free block that can handle `n` bytes
        BlockHeader *find_free_block(this Heap &self, lib::usize n);

        BlockHeader *create_block(lib::uptr base, lib::usize bytes, bool free, BlockHeader *prev, BlockHeader *next);
};

inline Heap kheap;

} /* namespace kernel::mem */