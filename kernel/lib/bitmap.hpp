#pragma once

#include <lib/print.hpp>
#include <lib/typing.hpp>

namespace kernel::lib {

template<usize N>
class StaticBitmap {
public:
        void set(this StaticBitmap &self, usize bit)
        {
                u64 index = self.get_index(bit);
                u64 mask = self.get_mask(bit);
                self.data[index] |= mask;
        }

        void clear(this StaticBitmap &self, usize bit)
        {
                u64 index = self.get_index(bit);
                u64 mask = self.get_mask(bit);
                self.data[index] &= ~mask;
        }

        bool test(this const StaticBitmap &self,usize bit)
        {
                u64 index = self.get_index(bit);
                u64 mask = self.get_mask(bit);
                return (self.data[index] & mask) != 0;
        }

        void set_all(this StaticBitmap &self)
        {
                for (usize i = 0; i < N; i++)
                        self.set(i);
        }

        void clear_all(this StaticBitmap &self)
        {
                for (usize i = 0; i < N; i++)
                        self.clear(i);
        }

        usize size() const
        {
                return N;
        }

private:
        u64 data[N];

        usize get_index(usize bit) const
        {
                usize index = bit / (sizeof(u64) * 8);
                return index;
        }

        usize get_mask(usize bit) const
        {
                usize mask = 1ull << (bit % (sizeof(u64) * 8));
                return mask;
        }
};

} /* namespace kernel::lib */