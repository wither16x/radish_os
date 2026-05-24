#include <lib/bytes.hpp>
#include <lib/logging.hpp>
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
        
        self.pages = 1;

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

        BlockHeader *block = self.find_free_block(n);
        if (!block) {
                usize required_pages = align_up(n + sizeof(BlockHeader), vmm.PageBytes) / vmm.PageBytes;
                for (usize i = 0; i < required_pages; i++)
                        self.extend();

                block = self.find_free_block(n);
                if (!block)
                        return nullptr; // out of memory
        }

        self.curr_block = block;
        self.split_block(n);
        block->free = false;

        return reinterpret_cast<void *>(reinterpret_cast<u8 *>(block) + sizeof(BlockHeader));
}

void Heap::extend(this Heap &self) {
        uptr new_page = HeapStart + vmm.PageBytes * self.pages;
        vmm.map_page(HeapStart + vmm.PageBytes * self.pages, pmm.allocate_frame(), 0x03 | (1ull << 63));
        self.pages++;

        BlockHeader *last_block = self.block_list;
        while (last_block->next_block_hdr)
                last_block = last_block->next_block_hdr;

        if (last_block->free) {
                last_block->bytes += vmm.PageBytes;
        } else {
                BlockHeader *new_block          = reinterpret_cast<BlockHeader *>(new_page);
                new_block->bytes                = vmm.PageBytes - sizeof(BlockHeader);
                new_block->free                 = true;
                new_block->next_block_hdr       = nullptr;
                new_block->previous_block_hdr   = last_block;

                last_block->next_block_hdr = new_block;
        }
}

void Heap::split_block(this Heap &self, lib::usize super_size)
{
        usize rem = self.curr_block->bytes - super_size;        // remaining bytes

        if (rem >= sizeof(BlockHeader) + 16) {
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

Heap::BlockHeader *Heap::find_free_block(this Heap &self, usize n)
{
        BlockHeader *current_block = self.block_list;

        while (current_block) {
                if (current_block->free && current_block->bytes >= n)
                        return current_block;

                current_block = current_block->next_block_hdr;
        }

        return nullptr;
}

} /* namespace kernel::mem */