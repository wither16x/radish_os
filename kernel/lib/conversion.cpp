#include <lib/conversion.hpp>
#include <lib/math.hpp>
#include <lib/memory.hpp>

namespace kernel::lib {

// --------------------------------------------------
char* itoa(isize n, char *str, int base)
{
        if (base < 2 or base > 36) {
                str[0] = '\0';
                return str;
        }

        usize u;

        bool neg = (n < 0) and (base == 10);

        if (n < 0)
                u = static_cast<usize>(-(n + 1)) + 1;
        else
                u = static_cast<usize>(n);

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
// --------------------------------------------------

// --------------------------------------------------
char* utoa(usize n, char *str, int base)
{
        if (base < 2 or base > 36) {
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
// --------------------------------------------------

// --------------------------------------------------
usize atoi(const char *s, int base)
{
        usize res = 0;
        int i = 0;

        for (; s[i] != '\0'; ++i)
                res = res * base + s[i] - '0';

        return res;
}
// --------------------------------------------------

// --------------------------------------------------
usize ctoi(char ch)
{
        return ch - '0';
}
// --------------------------------------------------

double atof(const char *s)
{
        int i = 0;
        int j = 0;
        int flag = 0;
        double value = 0;
        char c;

        while ((c = *(s + i)) != '\0') {
                if (c != '.') {
                        value = (value * 10) + (c - '0');
                        if (flag == 1)
                                --j;
                } else {
                        if (flag == 1)
                                return 0;

                        flag = 1;
                        ++i;
                }
        }

        value = value * pow<int>(10, j);
        return value;
}

char *ftoa(double n, char *buf)
{
        constexpr usize DECIMALS = 6;

        char before_point_buf[65];
        char after_point_buf[65];
        char padded[DECIMALS + 1];

        bool negative = n < 0;
        double abs_n = negative ? -n : n;

        i64 before_point = static_cast<i64>(abs_n);
        char *before_point_str = itoa(before_point, before_point_buf, 10);

        double after_point_f = (abs_n - before_point) * pow<usize>(10, DECIMALS);
        i64 after_point = static_cast<i64>(after_point_f);
        char *after_point_str = itoa(after_point, after_point_buf, 10);

        usize length = strlen(after_point_str);
        usize padding = (length < DECIMALS) ? (DECIMALS - length) : 0;

        usize i = 0;
        for(; i < padding; i++)
                padded[i] = '\0';
        strcpy(after_point_str, padded + i);

        usize pos = 0;
        if (negative)
                buf[pos++] = '-';

        usize bp_length = strlen(before_point_str);
        memcpy(buf + pos, before_point_str, bp_length);
        pos += bp_length;

        buf[pos++] = '.';

        memcpy(buf + pos, padded, DECIMALS);
        pos += DECIMALS;

        buf[pos] = '\0';

        return buf;
}

} /* namespace kernel::lib */