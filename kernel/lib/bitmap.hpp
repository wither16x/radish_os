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
                for (usize i = 0; i < Words; i++)
                        self.data[i] = ~0ull;

                constexpr usize rem = N % BitsPerWord;
                if constexpr (rem != 0)
                        self.data[Words - 1] &= (1ull << rem) - 1;
        }

        void clear_all(this StaticBitmap<N> &self)
        {
                for (usize i = 0; i < Words; i++)
                        self.data[i] = 0;
        }

        usize size() const
        {
                return N;
        }

private:
        static constexpr usize BitsPerWord      = sizeof(u64) * 8;
        static constexpr usize Words            = (N + BitsPerWord - 1) / BitsPerWord;

        u64 data[Words];

        static constexpr usize get_index(usize bit)
        {
                return bit / BitsPerWord;
        }

        static constexpr usize get_mask(usize bit)
        {
                return 1ull << (bit % BitsPerWord);
        }
};

} /* namespace kernel::lib */