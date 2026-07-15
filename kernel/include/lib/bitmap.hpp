#pragma once

#include <lib/vector.hpp>
#include <lib/typing.hpp>

// I have serious doubts on my bits/bytes operations not
// gonna lie.

namespace kernel::lib {

/// This bitmap has a fixed size known at compile-time.
template<usize N>
class StaticBitmap {
public:
        /// Set a bit.
        void set(this StaticBitmap<N> &self, usize bit)
        {
                if (bit < N)
                        self.data[self.get_index(bit)] |= self.get_mask(bit);
        }

        /// Clear a bit.
        void clear(this StaticBitmap<N> &self, usize bit)
        {
                if (bit < N)
                        self.data[self.get_index(bit)] &= ~self.get_mask(bit);
        }

        /// Get a bit.
        bool test(this const StaticBitmap<N> &self,usize bit)
        {
                if (bit < N)
                        return (self.data[self.get_index(bit)] & self.get_mask(bit)) != 0;
                return false;
        }

        /// Set all bits.
        void set_all(this StaticBitmap<N> &self)
        {
                for (usize i = 0; i < N; i++)
                        self.set(i);
        }

        /// Clear all bits.
        void clear_all(this StaticBitmap<N> &self)
        {
                for (usize i = 0; i < N; i++)
                        self.clear(i);
        }

        /// Get the size of the bitmap.
        usize size() const
        {
                return N;
        }

private:
        static constexpr usize BITS_PER_WORD      = sizeof(u64) * 8;
        static constexpr usize Words            = (N + BITS_PER_WORD - 1) / BITS_PER_WORD;

        u64 data[Words]{};

        /// Get the word of a bit in the bitmap.
        static constexpr usize get_index(usize bit)
        {
                return bit / BITS_PER_WORD;
        }

        /// Get the index of a bit in a word.
        static constexpr u64 get_mask(usize bit)
        {
                return 1ull << (bit % BITS_PER_WORD);
        }
};

/// This bitmap can extend itself.
/// ------------------------------------------------
/// I refactored this class by replacing the raw
/// pointer used to represent the bitmap data by
/// a Vector instance. So I guess that having a
/// length member is now useless, then consider the
/// refactoring as unfinished.
class DynamicBitmap {
public:
        void init(this DynamicBitmap &self, usize len)
        {
                self.data.resize((len + BITS_PER_WORD - 1) / BITS_PER_WORD);
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
                if (bit < self.length) {
                        usize idx = self.get_index(bit);
                        u64 mask = self.get_mask(bit);
                        u64 word = self.data[idx] & mask;
                        bool ret = word != 0;
                        return ret;
                }

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

        usize size(this const DynamicBitmap &self)
        {
                return self.length;
        }

private:
        static constexpr usize BITS_PER_WORD = sizeof(u64) * 8;

        usize length;
        Vector<u64> data;

        static constexpr usize get_index(usize bit)
        {
                return bit / BITS_PER_WORD;
        }

        static constexpr u64 get_mask(usize bit)
        {
                return 1ull << (bit % BITS_PER_WORD);
        }
};

} /* namespace kernel::lib */