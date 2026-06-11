#pragma once

#include <mem/allocators/allocator.hpp>
#include <lib/bitmap.hpp>
#include <lib/typing.hpp>
#include <panic.hpp>

namespace kernel::mem::allocators {

template<typename T>
class DynamicBitmapAllocator : public Allocator<T> {
private:
        lib::DynamicBitmap bitmap;

        lib::usize last_allocated = 0;

public:
        T allocate() override
        {
                lib::usize start = this->last_allocated;

                do {
                        if (!this->bitmap.test(this->last_allocated))
                                break;
                        
                        this->last_allocated++;

                        if (this->last_allocated >= this->bitmap.size())
                                this->bitmap.extend();
                } while (this->last_allocated != start);

                this->bitmap.set(this->last_allocated);

                return this->last_allocated;
        }

        void free(T n) override
        {
                if (!this->bitmap.test(n))
                        panic("double free");
                this->bitmap.clear(n);
        }

        lib::DynamicBitmap &get_bitmap(this DynamicBitmapAllocator<T> &self)
        {
                return self.bitmap;
        }
};

} /* namespace kernel::mem::allocators */