#pragma once

#include <lib/typing.hpp>

namespace kernel::lib {

extern "C" {

void *memcpy(void *__restrict dest, const void *__restrict src, usize n);
void *memset(void *s, int c, usize n);
void *memmove(void *dest, const void *src, usize n);
int memcmp(const void *s1, const void *s2, usize n);

}

} /* namespace kernel::lib */