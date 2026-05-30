#pragma once

#include <lib/alloc.hpp>
#include <lib/print.hpp>
#include <lib/typing.hpp>

namespace kernel::lib {

// This bitmap has a fixed size known at compile-time
template<usize N>
class StaticBitmap {
public:
        void set(this StaticBitmap<N> &self, usize bit)
        {
                if (bit < N)
                        self.data[self.get_index(bit)] |= self.get_mask(bit);
        }

        void clear(this StaticBitmap<N> &self, usize bit)
        {
                if (bit < N)
                        self.data[self.get_index(bit)] &= ~self.get_mask(bit);
        }

        bool test(this const StaticBitmap<N> &self,usize bit)
        {
                if (bit < N)
                        return (self.data[self.get_index(bit)] & self.get_mask(bit)) != 0;
                return false;
        }

        void set_all(this StaticBitmap<N> &self)
        {
                for (usize i = 0; i < N; i++)
                        self.set(i);
        }

        void clear_all(this StaticBitmap<N> &self)
        {
                for (usize i = 0; i < N; i++)
                        self.clear(i);
        }

        usize size() const
        {
                return N;
        }

private:
        static constexpr usize BitsPerWord      = sizeof(u64) * 8;
        static constexpr usize Words            = (N + BitsPerWord - 1) / BitsPerWord;

        u64 data[Words]{};

        static constexpr usize get_index(usize bit)
        {
                return bit / BitsPerWord;
        }

        static constexpr u64 get_mask(usize bit)
        {
                return 1ull << (bit % BitsPerWord);
        }
};

// This bitmap can extend itself
class DynamicBitmap {
public:
        ~DynamicBitmap()
        {
                delete[] this->data;
        }

        void init(this DynamicBitmap &self, usize len)
        {
                self.data = new u64[len];
                self.length = len;
        }

        void set(this DynamicBitmap &self, usize bit)
        {
                if (bit < self.length)
                        self.data[self.get_index(bit)] |= self.get_mask(bit);
        }

        void clear(this DynamicBitmap &self, usize bit)
        {
                if (bit < self.length)
                        self.data[self.get_index(bit)] &= ~self.get_mask(bit);
        }

        bool test(this const DynamicBitmap &self,usize bit)
        {
                if (bit < self.length)
                        return (self.data[self.get_index(bit)] & self.get_mask(bit)) != 0;
                return false;
        }

        void set_all(this DynamicBitmap &self)
        {
                for (usize i = 0; i < self.length; i++)
                        self.set(i);
        }

        void clear_all(this DynamicBitmap &self)
        {
                for (usize i = 0; i < self.length; i++)
                        self.clear(i);
        }

        // Add bits to the bitmap
        void extend(this DynamicBitmap &self, usize count)
        {
                self.length += count;
                u64 *new_data = new u64[self.length];
                for (usize i = 0; i < self.length; ++i)
                        new_data[i] = self.data[i];
                delete[] self.data;
                self.data = new_data;
        }

        usize size(this const DynamicBitmap &self)
        {
                return self.length;
        }

private:
        static constexpr usize BitsPerWord      = sizeof(u64) * 8;

        // Since the length of the bitmap can change, the amount of words
        // cannot be a constant like in `StaticBitmap` 
        usize length;

        u64 *data;

        static constexpr usize get_index(usize bit)
        {
                return bit / BitsPerWord;
        }

        static constexpr u64 get_mask(usize bit)
        {
                return 1ull << (bit % BitsPerWord);
        }
};

} /* namespace kernel::lib */