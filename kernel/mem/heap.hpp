#pragma once

#include <lib/typing.hpp>

namespace kernel::mem {

class Heap {
public:
        void init(this Heap &self);

        // Allocate `n` bytes
        void *allocate(this Heap &self, lib::usize n);

private:
        struct BlockHeader {
                lib::usize bytes;
                bool free;
                BlockHeader *next_block_hdr;
                BlockHeader *previous_block_hdr;
        };

        BlockHeader *block_list;
        BlockHeader *curr_block;

        lib::usize pages;               // amount of allocated pages

        // This function may not split the block if it is not
        // necessary
        void split_block(this Heap &self, lib::usize super_size);
};

inline Heap kheap;

} /* namespace kernel::mem */