#include <lib/bytes.hpp>
#include <lib/logging.hpp>
#include <lib/print.hpp>
#include <mem/heap.hpp>
#include <mem/pmm.hpp>
#include <mem/vmm.hpp>
#include <panic.hpp>

using kernel::lib::uptr, kernel::lib::usize, kernel::lib::u8;
using kernel::lib::log::logger;
using kernel::lib::align_up;

namespace kernel::mem {

namespace {

constexpr uptr HeapStart = 0xfffffe8000000000ull;

} /* annonymous namespace */

void Heap::init(this Heap &self)
{
        self.block_list.set_base(HeapStart);

        vmm::map_page(HeapStart, pmm::allocate_frame(), 0x03 | (1ull << 63));
        self.pages = 1;

        BlockHeader *new_list = self.block_list.first();
        new_list->bytes = vmm::PageBytes - sizeof(BlockHeader);
        new_list->free = true;

        logger.ok("initialized heap");
}

void *Heap::allocate(this Heap &self, usize n)
{
        n = align_up(n, 16);   // align `n` up

        BlockHeader *block = self.find_free_block(n);
        if (!block) {
                usize required_pages = align_up(n + sizeof(BlockHeader), vmm::PageBytes) / vmm::PageBytes;
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

void Heap::free(this Heap &self, void *p)
{
        BlockHeader *hdr = reinterpret_cast<BlockHeader *>(reinterpret_cast<u8 *>(p) - sizeof(BlockHeader));

        if (hdr->free)
                panic("double free on heap pointer on address 0x%x", p);

        hdr->free = true;

        if (hdr->next && static_cast<BlockHeader *>(hdr->next)->free) {
                hdr->bytes += sizeof(BlockHeader) + static_cast<BlockHeader *>(hdr->next)->bytes;
                hdr->next = hdr->next->next;
                if (hdr->next)
                        hdr->next->prev = hdr;
        }

        if (hdr->prev && static_cast<BlockHeader *>(hdr->prev)->free) {
                static_cast<BlockHeader *>(hdr->prev)->bytes += sizeof(BlockHeader) + hdr->bytes;
                hdr->prev->next = hdr->next;
                if (hdr->next)
                        hdr->next->prev = hdr->prev;
                hdr = static_cast<BlockHeader *>(hdr->prev);
        }

        while (self.pages > 1) {
                BlockHeader *last_block = self.block_list.last();

                if (!last_block->free)
                        break;

                if (!self.shorten())
                        break;
        }
}

void Heap::extend(this Heap &self) {
        uptr new_page = HeapStart + vmm::PageBytes * self.pages;
        vmm::map_page(HeapStart + vmm::PageBytes * self.pages, pmm::allocate_frame(), 0x03 | (1ull << 63));
        self.pages++;

        BlockHeader *last_block = self.block_list.last();

        if (last_block->free) {
                last_block->bytes += vmm::PageBytes;
        } else {
                BlockHeader *new_block = self.create_block(new_page, vmm::PageBytes, true, last_block, nullptr);
                self.block_list.append(new_block);
        }
}

bool Heap::shorten(this Heap &self)
{
        if (self.pages <= 1)
                return false;

        uptr last_page = HeapStart + vmm::PageBytes * (self.pages - 1);

        BlockHeader *curr = self.block_list.first();
        while (curr) {
                uptr curr_addr = reinterpret_cast<uptr>(curr);
                if (!curr->free && curr_addr >= last_page)
                        return false;
                curr = static_cast<BlockHeader *>(curr->next);
        }

        BlockHeader *last_block = self.block_list.last();

        if (!last_block->free)
                return false;

        uptr last_addr  = reinterpret_cast<uptr>(last_block);
        uptr last_start = last_addr + sizeof(BlockHeader);

        if (last_addr >= last_page) {
                if (last_block->prev)
                        last_block->prev->next = nullptr;
                else
                        self.block_list.reset();
        } else {
                last_block->bytes = last_page - last_start;
                last_block->next  = nullptr;
        }

        vmm::unmap_page(last_page);
        self.pages--;
        
        return true;
}

void Heap::split_block(this Heap &self, lib::usize super_size)
{
        usize rem = self.curr_block->bytes - super_size;        // remaining bytes

        if (rem >= sizeof(BlockHeader) + 16) {
                uptr block_base = reinterpret_cast<uptr>(reinterpret_cast<u8 *>(self.curr_block) + sizeof(BlockHeader) + super_size);
                BlockHeader *new_block = self.create_block(block_base, rem, true, self.curr_block, static_cast<BlockHeader *>(self.curr_block->next));

                if (self.curr_block->next)
                        self.curr_block->next->prev = new_block;

                self.curr_block->next = new_block;
                self.curr_block->bytes = super_size;
        }
}

BlockHeader *Heap::find_free_block(this Heap &self, usize n)
{
        BlockHeader *current_block = self.block_list.first();

        while (current_block) {
                if (current_block->free && current_block->bytes >= n)
                        return current_block;

                current_block = static_cast<BlockHeader *>(current_block->next);
        }

        return nullptr;
}

BlockHeader *Heap::create_block(lib::uptr base, lib::usize bytes, bool free, BlockHeader *prev, BlockHeader *next)
{
        BlockHeader *block = reinterpret_cast<BlockHeader *>(base);
        block->bytes = bytes - sizeof(BlockHeader);
        block->free = free;
        block->prev = prev;
        block->next = next;

        return block;
}

} /* namespace kernel::mem */