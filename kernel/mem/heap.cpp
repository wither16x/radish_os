#include <lib/bytes.hpp>
#include <lib/logging.hpp>
#include <lib/print.hpp>
#include <mem/heap.hpp>
#include <mem/pmm.hpp>
#include <mem/vmm.hpp>

using namespace kernel::lib;

namespace kernel::mem {

namespace {

constexpr uptr HeapStart = 0xfffffe8000000000ull;

}

void Heap::init(this Heap &self)
{
        log::status("initializing heap");

        vmm.map_page(HeapStart, pmm.allocate_frame(), 0x03 | (1ull << 63));
        
        self.block_list = reinterpret_cast<BlockHeader *>(HeapStart);
        self.block_list->bytes = vmm.PageBytes - sizeof(BlockHeader);
        self.block_list->free = true;
        self.block_list->next_block_hdr = nullptr;
        self.block_list->previous_block_hdr = nullptr;

        log::ok();
}

void *Heap::allocate(this Heap &self, usize n)
{
        n = align_up(n, 16);   // align `n` up

        self.curr_block = self.block_list;

        while (self.curr_block) {
                if (self.curr_block->free && self.curr_block->bytes >= n) {
                        self.split_block(n);
                        self.curr_block->free = false;
                        return reinterpret_cast<void *>(reinterpret_cast<u8 *>(self.curr_block) + sizeof(BlockHeader));
                }

                self.curr_block = self.curr_block->next_block_hdr;
        }

        return nullptr;
}

void Heap::split_block(this Heap &self, lib::usize super_size)
{
        usize rem = self.curr_block->bytes - super_size;        // remaining bytes

        if (rem >= sizeof(BlockHeader) * 16) {
                BlockHeader *new_block = reinterpret_cast<BlockHeader *>(
                        reinterpret_cast<u8 *>(self.curr_block) + sizeof(BlockHeader) + super_size
                );
                new_block->bytes = rem - sizeof(BlockHeader);
                new_block->free = true;
                new_block->next_block_hdr = self.curr_block->next_block_hdr;
                new_block->previous_block_hdr = self.curr_block;

                if (self.curr_block->next_block_hdr)
                        self.curr_block->next_block_hdr->previous_block_hdr = new_block;

                self.curr_block->next_block_hdr = new_block;
                self.curr_block->bytes = super_size; 
        }
}

} /* namespace kernel::mem */