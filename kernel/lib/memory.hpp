#pragma once

extern "C" {

void *memcpy(void *__restrict dest, const void *__restrict src, unsigned long long n);
void *memset(void *s, int c, unsigned long long n);
void *memmove(void *dest, const void *src, unsigned long long n);
int memcmp(const void *s1, const void *s2, unsigned long long n);

}