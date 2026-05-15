#include <lib/memory.hpp>

extern "C" {

void *memcpy(void *__restrict dest, const void *__restrict src, unsigned long long n) {
    unsigned char *__restrict pdest = static_cast<unsigned char *__restrict>(dest);
    const unsigned char *__restrict psrc = static_cast<const unsigned char *__restrict>(src);

    for (unsigned long long i = 0; i < n; i++) {
        pdest[i] = psrc[i];
    }

    return dest;
}

void *memset(void *s, int c, unsigned long long n) {
    unsigned char *p = static_cast<unsigned char *>(s);

    for (unsigned long long i = 0; i < n; i++) {
        p[i] = static_cast<unsigned char>(c);
    }

    return s;
}

void *memmove(void *dest, const void *src, unsigned long long n) {
    unsigned char *pdest = static_cast<unsigned char *>(dest);
    const unsigned char *psrc = static_cast<const unsigned char *>(src);

    if (reinterpret_cast<unsigned long long>(src) > reinterpret_cast<unsigned long long>(dest)) {
        for (unsigned long long i = 0; i < n; i++) {
            pdest[i] = psrc[i];
        }
    } else if (reinterpret_cast<unsigned long long>(src) < reinterpret_cast<unsigned long long>(dest)) {
        for (unsigned long long i = n; i > 0; i--) {
            pdest[i-1] = psrc[i-1];
        }
    }

    return dest;
}

int memcmp(const void *s1, const void *s2, unsigned long long n) {
    const unsigned char *p1 = static_cast<const unsigned char *>(s1);
    const unsigned char *p2 = static_cast<const unsigned char *>(s2);

    for (unsigned long long i = 0; i < n; i++) {
        if (p1[i] != p2[i]) {
            return p1[i] < p2[i] ? -1 : 1;
        }
    }

    return 0;
}

}