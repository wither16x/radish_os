#pragma once

namespace kernel::mem::allocators {

// Base class for all allocators
template<typename T>
class Allocator {
public:
        virtual T allocate() = 0;
        virtual void free(T n) = 0;
};

} /* namespace kernel::mem::allocators */