#pragma once

#include <mem/allocators/allocator.hpp>
#include <lib/bitmap.hpp>
#include <lib/typing.hpp>
#include <panic.hpp>

namespace kernel::mem::allocators {

/// Allocator based on a dynamic bitmap.
template<typename T>
class DynamicBitmapAllocator : public Allocator<T> {
private:
        lib::DynamicBitmap bitmap;

        lib::usize last_allocated = 0;

public:
        T allocate(lib::usize n) override
        {
                for (lib::usize i = 0; i < n; i++) {
                        lib::usize start = this->last_allocated;

                        do {
                                bool state = this->bitmap.test(this->last_allocated);
                                if (!state)
                                        break;
                                
                                this->last_allocated++;

                                if (this->last_allocated >= this->bitmap.size())
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

        /// Return the `DynamicBitmap` instance.
        lib::DynamicBitmap &get_bitmap(this DynamicBitmapAllocator<T> &self)
        {
                return self.bitmap;
        }
};

} /* namespace kernel::mem::allocators */