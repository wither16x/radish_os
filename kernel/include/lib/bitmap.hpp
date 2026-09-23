#pragma once

#include <lib/vector.hpp>
#include <lib/typing.hpp>
#include <lib/array.hpp>

namespace Kiwi::Lib
{
        /// This bitmap has a fixed size known at compile-time.
        template<usize N>
        class StaticBitmap
        {
                static constexpr usize BITS_PER_WORD      = sizeof(u64) * 8;
                static constexpr usize WORDS              = (N + BITS_PER_WORD - 1) / BITS_PER_WORD;

                u64 data[WORDS]{};

                /// Get the word of a bit in the bitmap.
                static constexpr usize getIndex(usize bit)
                {
                        return bit / BITS_PER_WORD;
                }

                /// Get the index of a bit in a word.
                static constexpr u64 getMask(usize bit)
                {
                        return 1ull << (bit % BITS_PER_WORD);
                }

        public:
                /// Set a bit.
                void set(this StaticBitmap<N> &self, usize bit)
                {
                        if (bit < N)
                                self.data[self.getIndex(bit)] |= self.getMask(bit);
                }

                /// Clear a bit.
                void clear(this StaticBitmap<N> &self, usize bit)
                {
                        if (bit < N)
                                self.data[self.getIndex(bit)] &= ~self.getMask(bit);
                }

                /// Get a bit.
                bool test(this const StaticBitmap<N> &self,usize bit)
                {
                        if (bit < N)
                                return (self.data[self.getIndex(bit)] & self.getMask(bit)) != 0;
                        return false;
                }

                /// Set all bits.
                void setAll(this StaticBitmap<N> &self)
                {
                        for (usize i = 0; i < N; i++)
                                self.set(i);
                }

                /// Clear all bits.
                void clearAll(this StaticBitmap<N> &self)
                {
                        for (usize i = 0; i < N; i++)
                                self.clear(i);
                }

                /// Get the size of the bitmap.
                usize size() const
                {
                        return N;
                }
        };
} // namespace Kiwi::Lib