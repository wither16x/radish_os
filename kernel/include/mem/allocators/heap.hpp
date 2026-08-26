#pragma once

#include <kernel.hpp>
#include <lib/bytes.hpp>
#include <lib/linked_list.hpp>
#include <lib/typing.hpp>
#include <mem/allocators/allocator.hpp>
#include <mem/pmm.hpp>
#include <mem/vmm.hpp>
#include <mem/pml4t.hpp>
#include <mem/page.hpp>
#include <panic.hpp>

namespace Kiwi::Mem::Allocators
{
        /// Allocator designed to manage memory on a heap or any
        /// other extensible memory area.
        template<typename T>
        class HeapAllocator : public Allocator<T>
        {
                struct BlockHeader : public Lib::LinkedListHeader
                {
                        Lib::usize bytes;
                        bool free;
                };

                Lib::uptr base;
                Lib::usize alignment;
                Lib::usize pages;
                Lib::LinkedList<BlockHeader> block_list;
                BlockHeader *curr_block;

                /// Map more pages to handle more blocks.
                void extend(this HeapAllocator<T> &self)
                {
                        Lib::uptr new_page = self.base + PAGE_SIZE * self.pages;

                        getKernelPml4t().mapPage(
                                new_page,
                                Pmm::allocateFrame(),
                                PageFlag::ReadWriteUser | PageFlag::NoExec
                        );
                        ++self.pages;

                        BlockHeader *last_block = self.block_list.last();

                        if (last_block->free) {
                                last_block->bytes += PAGE_SIZE;
                        } else {
                                BlockHeader *new_block = self.createBlock(new_page, PAGE_SIZE, true, last_block, nullptr);
                                self.block_list.append(new_block);
                        }
                }

                /// Unmap empty pages.
                bool shorten(this HeapAllocator<T> &self)
                {
                        if (self.pages <= 1)
                                return false;

                        Lib::uptr last_page = self.base + PAGE_SIZE * (self.pages - 1);

                        BlockHeader *curr = self.block_list.first();
                        while (curr) {
                                Lib::uptr curr_addr = reinterpret_cast<Lib::uptr>(curr);
                                Lib::uptr curr_end = curr_addr + sizeof(BlockHeader) + curr->bytes;

                                if (not curr->free and curr_end >= last_page)
                                        return false;

                                curr = static_cast<BlockHeader *>(curr->next);
                        }

                        BlockHeader *last_block = self.block_list.last();
                        if (not last_block->free)
                                return false;

                        Lib::uptr last_addr  = reinterpret_cast<Lib::uptr>(last_block);

                        if (last_addr >= last_page) {
                                if (last_block->prev)
                                        last_block->prev->next = nullptr;
                                else
                                        self.block_list.reset();
                        } else if (last_addr + sizeof(BlockHeader) <= last_page) {
                                last_block->bytes = last_page - (last_addr + sizeof(BlockHeader));
                                last_block->next = nullptr;
                        } else {
                                return false;
                        }

                        getKernelPml4t().unmapPage(last_page);
                        --self.pages;
                        
                        return true;
                }

                /// Split a block in two smaller parts.
                void splitBlock(this HeapAllocator<T> &self, Lib::usize base_size)
                {
                        Lib::usize rem = self.curr_block->bytes - base_size;        // remaining bytes

                        if (rem >= sizeof(BlockHeader) + self.alignment) {
                                Lib::uptr block_base = reinterpret_cast<Lib::uptr>(reinterpret_cast<Lib::u8 *>(self.curr_block) + sizeof(BlockHeader) + base_size);
                                BlockHeader *new_block = self.createBlock(block_base, rem, true, self.curr_block, static_cast<BlockHeader *>(self.curr_block->next));

                                if (self.curr_block->next)
                                        self.curr_block->next->prev = new_block;

                                self.curr_block->next = new_block;
                                if (not self.curr_block->next)
                                        panic("new block is null");
                                self.curr_block->bytes = base_size;
                        }
                }

                /// Find a free block which is big enough to handle `min_size`.
                BlockHeader *findFreeBlock(this HeapAllocator<T> &self, Lib::usize min_size)
                {
                        BlockHeader *current_block = self.block_list.first();

                        while (current_block) {
                                if (current_block->free and current_block->bytes >= min_size)
                                        return current_block;

                                current_block = static_cast<BlockHeader *>(current_block->next);
                        }

                        return nullptr;
                }

                /// Create a new block.
                BlockHeader *createBlock(Lib::uptr base, Lib::usize bytes, bool free, BlockHeader *prev, BlockHeader *next)
                {
                        BlockHeader *block = reinterpret_cast<BlockHeader *>(base);
                        block->bytes = bytes - sizeof(BlockHeader);
                        block->free = free;
                        block->prev = prev;
                        block->next = next;

                        return block;
                }

        public:
                /// Initialize the allocator.
                void init(this HeapAllocator<T> &self, Lib::uptr base, Lib::usize alignment)
                {
                        self.base = base;
                        self.alignment = alignment;
                        self.curr_block = nullptr;

                        getKernelPml4t().mapPage(
                                self.base,
                                Pmm::allocateFrame(),
                                PageFlag::ReadWriteUser | PageFlag::NoExec
                        );
                        self.pages = 1;

                        self.block_list.setBase(self.base);

                        BlockHeader *new_list = self.block_list.first();
                        new_list->bytes = PAGE_SIZE - sizeof(BlockHeader);
                        new_list->free = true;
                }

                T allocate(Lib::usize n) override
                {
                        n = Lib::alignUp(n, this->alignment);   // align `n` up

                        BlockHeader *block = this->findFreeBlock(n);

                        if (not block) {
                                Lib::usize required_pages = Lib::alignUp(n + sizeof(BlockHeader), PAGE_SIZE) / PAGE_SIZE;
                                for (Lib::usize i = 0; i < required_pages; i++)
                                        this->extend();

                                block = this->findFreeBlock(n);
                                if (!block)
                                        return nullptr; // out of memory
                        }
                                
                        this->curr_block = block;
                        this->splitBlock(n);
                        block->free = false;

                        return reinterpret_cast<void *>(reinterpret_cast<Lib::u8 *>(block) + sizeof(BlockHeader));
                }

                void free(T n) override
                {
                        BlockHeader *hdr = reinterpret_cast<BlockHeader *>(reinterpret_cast<Lib::u8 *>(n) - sizeof(BlockHeader));

                        if (hdr->free)
                                panic("double free");

                        hdr->free = true;

                        if (hdr->next and static_cast<BlockHeader *>(hdr->next)->free) {
                                hdr->bytes += sizeof(BlockHeader) + static_cast<BlockHeader *>(hdr->next)->bytes;
                                hdr->next = hdr->next->next;
                                if (hdr->next)
                                        hdr->next->prev = hdr;
                        }

                        if (hdr->prev and static_cast<BlockHeader *>(hdr->prev)->free) {
                                static_cast<BlockHeader *>(hdr->prev)->bytes += sizeof(BlockHeader) + hdr->bytes;
                                hdr->prev->next = hdr->next;
                                if (hdr->next)
                                        hdr->next->prev = hdr->prev;
                                hdr = static_cast<BlockHeader *>(hdr->prev);
                        }

                        while (this->pages > 1) {
                                BlockHeader *last_block = this->block_list.last();

                                if (not last_block->free)
                                        break;

                                if (not this->shorten())
                                        break;
                        }
                }
        };
} // namespace Kiwi::Mem::Allocators