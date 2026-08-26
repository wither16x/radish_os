#pragma once

#include <mem/allocators/allocator.hpp>
#include <lib/bitmap.hpp>
#include <lib/typing.hpp>
#include <panic.hpp>

namespace Kiwi::Mem::Allocators
{
        /// Allocator based on a static bitmap, so the maximum amount
        /// of memory it can handle is limited by `SIZE`.
        template<typename T, Lib::usize SIZE>
        class StaticBitmapAllocator : public Allocator<T>
        {
                Lib::StaticBitmap<SIZE> bitmap;
                Lib::usize last_allocated = 0;

        public:
                T allocate(Lib::usize n) override
                {
                        for (Lib::usize i = 0; i < n; i++) {
                                Lib::usize start = this->last_allocated;
                                bool found = false;

                                do {
                                        if (not this->bitmap.test(this->last_allocated)) {
                                                found = true;
                                                break;
                                        }

                                        ++this->last_allocated;

                                        if (this->last_allocated >= SIZE)
                                                this->last_allocated = 0;
                                } while (this->last_allocated != start);

                                if (not found)
                                        panic("out of memory");

                                this->bitmap.set(this->last_allocated);
                        }

                        // returns the value from the last allocation
                        return this->last_allocated;
                }

                void free(T n) override
                {
                        if (not this->bitmap.test(n))
                                panic("double free");

                        this->bitmap.clear(n);
                }

                /// Return the `StaticBitmap` instance.
                Lib::StaticBitmap<SIZE> &getBitmap(this StaticBitmapAllocator<T, SIZE> &self)
                {
                        return self.bitmap;
                }
        };
} // namespace Kiwi::Mem::Allocators