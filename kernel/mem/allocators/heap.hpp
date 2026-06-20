#pragma once

#include <lib/bytes.hpp>
#include <lib/linked_list.hpp>
#include <lib/typing.hpp>
#include <mem/allocators/allocator.hpp>
#include <mem/pmm.hpp>
#include <mem/vmm.hpp>
#include <panic.hpp>

namespace kernel::mem::allocators {

template<typename T>
class HeapAllocator : public Allocator<T> {
private:
        struct BlockHeader : public lib::LinkedListHeader {
                lib::usize bytes;
                bool free;
        };

        lib::uptr base;
        lib::usize alignment;

        lib::usize pages;

        lib::LinkedList<BlockHeader> block_list;
        BlockHeader *curr_block;

        // Map more pages to handle more blocks
        void extend(this HeapAllocator<T> &self)
        {
                lib::uptr new_page = self.base + vmm::PAGE_BYTES * self.pages;
                vmm::map_page(self.base + vmm::PAGE_BYTES * self.pages, pmm::allocate_frame(), 0x03 | (1ull << 63));
                self.pages++;

                BlockHeader *last_block = self.block_list.last();

                if (last_block->free) {
                        last_block->bytes += vmm::PAGE_BYTES;
                } else {
                        BlockHeader *new_block = self.create_block(new_page, vmm::PAGE_BYTES, true, last_block, nullptr);
                        self.block_list.append(new_block);
                }
        }

        // Unmap empty pages
        bool shorten(this HeapAllocator<T> &self)
        {
                if (self.pages <= 1)
                        return false;

                lib::uptr last_page = self.base + vmm::PAGE_BYTES * (self.pages - 1);

                BlockHeader *curr = self.block_list.first();
                while (curr) {
                        lib::uptr curr_addr = reinterpret_cast<lib::uptr>(curr);
                        if (!curr->free && curr_addr >= last_page)
                                return false;
                        curr = static_cast<BlockHeader *>(curr->next);
                }

                BlockHeader *last_block = self.block_list.last();

                if (!last_block->free)
                        return false;

                lib::uptr last_addr  = reinterpret_cast<lib::uptr>(last_block);
                lib::uptr last_start = last_addr + sizeof(BlockHeader);

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

        // Split a block in two parts
        void split_block(this HeapAllocator<T> &self, lib::usize base_size)
        {
                lib::usize rem = self.curr_block->bytes - base_size;        // remaining bytes

                if (rem >= sizeof(BlockHeader) + self.alignment) {
                        lib::uptr block_base = reinterpret_cast<lib::uptr>(reinterpret_cast<lib::u8 *>(self.curr_block) + sizeof(BlockHeader) + base_size);
                        BlockHeader *new_block = self.create_block(block_base, rem, true, self.curr_block, static_cast<BlockHeader *>(self.curr_block->next));

                        if (self.curr_block->next)
                                self.curr_block->next->prev = new_block;

                        self.curr_block->next = new_block;
                        self.curr_block->bytes = base_size;
                }
        }

        // Find a free block which is big enough or bigger than `min_size` 
        BlockHeader *find_free_block(this HeapAllocator<T> &self, lib::usize min_size)
        {
                BlockHeader *current_block = self.block_list.first();

                while (current_block) {
                        if (current_block->free && current_block->bytes >= min_size)
                                return current_block;

                        current_block = static_cast<BlockHeader *>(current_block->next);
                }

                return nullptr;
        }

        // Create a new block
        BlockHeader *create_block(lib::uptr base, lib::usize bytes, bool free, BlockHeader *prev, BlockHeader *next)
        {
                BlockHeader *block = reinterpret_cast<BlockHeader *>(base);
                block->bytes = bytes - sizeof(BlockHeader);
                block->free = free;
                block->prev = prev;
                block->next = next;

                return block;
        }

public:
        // Initialize the allocator
        void init(this HeapAllocator<T> &self, lib::uptr base, lib::usize alignment)
        {
                self.base = base;
                self.alignment = alignment;

                self.block_list.set_base(self.base);

                vmm::map_page(self.base, pmm::allocate_frame(), vmm::PageFlag::ReadWrite | vmm::PageFlag::NoExec);
                self.pages = 1;

                BlockHeader *new_list = self.block_list.first();
                new_list->bytes = vmm::PAGE_BYTES - sizeof(BlockHeader);
                new_list->free = true;
        }

        // Allocate `n` bytes and return a pointer to the base region in the heap
        T allocate(lib::usize n) override
        {
                n = lib::align_up(n, this->alignment);   // align `n` up

                BlockHeader *block = this->find_free_block(n);
                if (!block) {
                        lib::usize required_pages = lib::align_up(n + sizeof(BlockHeader), vmm::PAGE_BYTES) / vmm::PAGE_BYTES;
                        for (lib::usize i = 0; i < required_pages; i++)
                                this->extend();

                        block = this->find_free_block(n);
                        if (!block)
                                return nullptr; // out of memory
                }

                this->curr_block = block;
                this->split_block(n);
                block->free = false;

                return reinterpret_cast<void *>(reinterpret_cast<lib::u8 *>(block) + sizeof(BlockHeader));
        }

        // Free an allocated pointer (panic if the pointer is already free)
        void free(T n) override
        {
                BlockHeader *hdr = reinterpret_cast<BlockHeader *>(reinterpret_cast<lib::u8 *>(n) - sizeof(BlockHeader));

                if (hdr->free)
                        panic("double free");

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

                while (this->pages > 1) {
                        BlockHeader *last_block = this->block_list.last();

                        if (!last_block->free)
                                break;

                        if (!this->shorten())
                                break;
                }
        }
};

} /* namespace kernel::mem::allocators */