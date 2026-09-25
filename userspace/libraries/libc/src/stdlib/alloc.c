#include "__utils/decls.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"

// The RadishOS kernel only supports 4KiB pages
// TODO: make a sysconf syscall to get this kind of values
#define __PAGE_SIZE     0x1000

struct heap_block {
        size_t size;
        struct heap_block *next;
        bool is_free;
};

static struct heap_block *heap_start = NULL;
static struct heap_block *heap_tail = NULL;

BEGIN_DECLS

static struct heap_block *find_free_block(size_t size)
{
        struct heap_block *curr = heap_start;

        while (curr) {
                if (curr->is_free && curr->size >= size)
                        return curr;
                
                curr = curr->next;
        }

        return NULL;
}

static struct heap_block *request_memory(size_t size)
{
        size_t total_size = sizeof(struct heap_block) + size;

        size_t page_size = __PAGE_SIZE;
        size_t aligned_size = ((total_size + page_size - 1) / page_size) * page_size;
        size_t page_count = aligned_size / page_size;

        void *block = lastpg(page_count);
        if (!block)
                return NULL;

        struct heap_block *new_block = (struct heap_block *)block;
        new_block->size = aligned_size - sizeof(struct heap_block);
        new_block->is_free = false;
        new_block->next = NULL;

        if (heap_start == NULL)
                heap_start = new_block;
        else
                heap_tail->next = new_block;

        heap_tail = new_block;

        return new_block;
}

static void split_block(struct heap_block *block, size_t size)
{
        if (block->size - size > sizeof(struct heap_block) + 16) {
                struct heap_block *new_block = (struct heap_block *)((char *)block + sizeof(struct heap_block) + size);
                new_block->size = block->size - size - sizeof(struct heap_block);
                new_block->is_free = true;
                new_block->next = block->next;

                block->size = size;
                block->next = new_block;

                if (heap_tail == block)
                        heap_tail = new_block;
        }
}

static void coalesce_blocks(struct heap_block *block)
{
        if (block->next && block->next->is_free) {
                if (heap_tail == block->next)
                        heap_tail = block;
                
                block->size += sizeof(struct heap_block) + block->next->size;
                block->next = block->next->next;
        }

        struct heap_block *curr = heap_start;
        while (curr && curr->next != block)
                curr = curr->next;

        if (curr && curr->is_free) {
                if (heap_tail == block)
                        heap_tail = curr;

                curr->size += sizeof(struct heap_block) + block->size;
                curr->next = block->next;
        }
}

/* ------------------------------------------------------------------------------------------------- */
void *malloc(size_t size)
{
        if (size == 0)
                return NULL;

        size = (size + 7) & ~7;
        
        struct heap_block *block = find_free_block(size);
        if (block) {
                split_block(block, size);
                block->is_free = false;
        } else {
                block = request_memory(size);
                if (!block)
                        return NULL;
        }

        return (void *)((char *)block + sizeof(struct heap_block));
}
/* ------------------------------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------------------------------- */
void *calloc(size_t num, size_t size)
{
        size_t sz = num * size;
        void *p = malloc(sz);
        if (!p)
                return NULL;

        memset(p, 0, sz);
        return p;
}
/* ------------------------------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------------------------------- */
void *realloc(void *ptr, size_t new_size)
{
        if (!ptr)
                return malloc(new_size);

        struct heap_block *block = (struct heap_block *)ptr;
        if (block->size >= new_size)
                return ptr;

        void *new_ptr = malloc(new_size);
        if (!new_ptr)
                return NULL;

        memcpy(new_ptr, ptr, block->size);
        free(ptr);

        return new_ptr;
}
/* ------------------------------------------------------------------------------------------------- */

void *aligned_alloc(size_t alignment, size_t size)
{
        void *p1;
        void **p2;
        int offset = alignment - 1 + sizeof(void *);

        if ((p1 = (void *)malloc(size + offset)) == NULL)
                return NULL;

        p2 = (void *)(((size_t)(p1) + offset) & ~(alignment - 1));
        p2[-1] = p1;

        return p2;
}

/* ------------------------------------------------------------------------------------------------- */
void free(void *ptr)
{
        if (!ptr)
                return;

        struct heap_block *block = (struct heap_block *)((char *)ptr - sizeof(struct heap_block));
        block->is_free = true;

        coalesce_blocks(block);
}
/* ------------------------------------------------------------------------------------------------- */

END_DECLS