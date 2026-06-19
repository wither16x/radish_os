#include <lib/conversion.hpp>

namespace kernel::lib {

char* itoa(isize n, int base)
{
        static char str[65];

        if (base < 2 || base > 36) {
                str[0] = '\0';
                return str;
        }

        usize u;

        bool neg = (n < 0) && (base == 10);

        if (n < 0)
                u = (usize)(-(n + 1)) + 1;
        else
                u = (usize)n;

        char* p = str;

        do {
                *p++ = "0123456789abcdefghijklmnopqrstuvwxyz"[u % base];
                u /= base;
        } while (u);

        if (neg)
                *p++ = '-';

        *p = '\0';

        char* start = str;
        char* end = p - 1;

        while (start < end) {
                char tmp = *start;
                *start++ = *end;
                *end-- = tmp;
        }

        return str;
}

char* utoa(usize n, int base)
{
        static char str[65];

        if (base < 2 || base > 36) {
                str[0] = '\0';
                return str;
        }

        char* p = str;

        do {
                *p++ = "0123456789abcdefghijklmnopqrstuvwxyz"[n % base];
                n /= base;
        } while (n);

        *p = '\0';

        char* start = str;
        char* end = p - 1;

        while (start < end) {
                char tmp = *start;
                *start++ = *end;
                *end-- = tmp;
        }

        return str;
}

usize atoi(const char *s, int base)
{
        usize res = 0;
        int i = 0;

        for (; s[i] != '\0'; ++i)
                res = res * base + s[i] - '0';

        return res;
}

usize ctoi(char ch)
{
        return ch - '0';
}

} /* namespace kernel::lib */