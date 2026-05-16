#pragma once

#include <lib/print.hpp>
#include <lib/typing.hpp>

namespace kernel::lib {

template<usize N>
class StaticBitmap {
public:
        void set(usize bit)
        {
                u64 index = this->get_index(bit);
                u64 mask = this->get_mask(bit);
                this->data[index] |= mask;
        }

        void clear(usize bit)
        {
                u64 index = this->get_index(bit);
                u64 mask = this->get_mask(bit);
                this->data[index] &= ~mask;
        }

        bool test(usize bit) const
        {
                u64 index = this->get_index(bit);
                u64 mask = this->get_mask(bit);
                return (this->data[index] & mask) != 0;
        }

        void set_all()
        {
                for (usize i = 0; i < N * (sizeof(u64) * 8); i++)
                        this->set(i);
        }

        void clear_all()
        {
                for (usize i = 0; i < N * (sizeof(u64) * 8); i++)
                        this->clear(i);
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