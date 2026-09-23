#pragma once

#include <lib/typing.hpp>

void *operator new(size_t size);
void operator delete(void *ptr);

void *operator new[](size_t size);
void operator delete[](void *ptr);

inline void *operator new(size_t, void *p)
{
        return p;
}

inline void operator delete(void *, void *) {}

inline void *operator new[](size_t, void *p)
{
        return p;
}

inline void operator delete[](void *, void *) {}