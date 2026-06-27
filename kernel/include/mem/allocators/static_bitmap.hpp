#pragma once

#include <mem/allocators/allocator.hpp>
#include <lib/bitmap.hpp>
#include <lib/typing.hpp>
#include <panic.hpp>

namespace kernel::mem::allocators {

/// Allocator based on a static bitmap, so the maximum amount
/// of memory it can handle is limited by `SIZE`.
template<typename T, lib::usize SIZE>
class StaticBitmapAllocator : public Allocator<T> {
private:
        lib::StaticBitmap<SIZE> bitmap;

        lib::usize last_allocated = 0;

public:
        T allocate(lib::usize n) override
        {
                for (lib::usize i = 0; i < n; i++) {
                        lib::usize start = this->last_allocated;

                        do {
                                if (!this->bitmap.test(this->last_allocated))
                                        break;

                                this->last_allocated++;

                                if (this->last_allocated >= SIZE)
                                        panic("out of memory");
                        } while (this->last_allocated != start);

                        this->bitmap.set(this->last_allocated);
                }

                // returns the value from the last allocation
                return this->last_allocated;
        }

        void free(T n) override
        {
                if (!this->bitmap.test(n))
                        panic("double free");

                this->bitmap.clear(n);
        }

        /// Return the `StaticBitmap` instance.
        lib::StaticBitmap<SIZE> &get_bitmap(this StaticBitmapAllocator<T, SIZE> &self)
        {
                return self.bitmap;
        }
};

} /* namespace kernel::mem::allocators */