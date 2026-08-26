#pragma once

#include <mem/allocators/allocator.hpp>
#include <lib/bitmap.hpp>
#include <lib/typing.hpp>
#include <panic.hpp>

namespace Kiwi::Mem::Allocators
{
        /// THIS ALLOCATOR IS DEPRECATED!!!
        /// Allocator based on a dynamic bitmap.
        template<typename T>
        class DynamicBitmapAllocator : public Allocator<T>
        {
                Lib::DynamicBitmap bitmap;
                Lib::usize last_allocated = 0;

        public:
                T allocate(Lib::usize n) override
                {
                        for (Lib::usize i = 0; i < n; i++) {
                                Lib::usize start = this->last_allocated;

                                do {
                                        bool state = this->bitmap.test(this->last_allocated);
                                        if (not state)
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
                        if (not this->bitmap.test(n))
                                panic("double free");
                        this->bitmap.clear(n);
                }

                /// Return the `DynamicBitmap` instance.
                Lib::DynamicBitmap &getBitmap(this DynamicBitmapAllocator<T> &self)
                {
                        return self.bitmap;
                }
        };

} // namespace Kiwi::Mem::Allocators