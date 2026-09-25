#include "__utils/decls.h"
#include "stdio.h"
#include "unistd.h"
#include "string.h"
#include "stdbool.h"
#include "math.h"

#define FMT_CHAR        'c'
#define FMT_STR         's'
#define FMT_DECIMAL     'd'
#define FMT_BINARY      'b'
#define FMT_OCTAL       'o'
#define FMT_HEXADECIMAL 'x'
#define FMT_UNSIGNED    'u'
#define FMT_DOUBLE      'f'

BEGIN_DECLS

/* ------------------------------------------------------------------------------------------------- */
static int fprints(const char *s, FILE *stream)
{
        return write(fileno(stream), (const void *)s, strlen(s));
}
/* ------------------------------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------------------------------- */
static void strreverse(char *str, int length)
{
        int start = 0;
        int end = length - 1;

        while (start < end) {
                char tmp = str[start];
                str[start] = str[end];
                str[end] = tmp;
                end--;
                start++;
        }
}
/* ------------------------------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------------------------------- */
static char *itoa(long long num, char *buf, int base)
{
        int i = 0;
        bool is_negative = false;

        if (num == 0) {
                buf[i++] = '0';
                buf[i] = '\0';
                return buf;
        }

        if (num < 0 && base == 10) {
                is_negative = true;
                num = -num;
        }

        while (num != 0) {
                long long remaining = num % base;
                buf[i++] = (remaining > 9) ? (remaining - 10) + 'a' : remaining + '0';
                num = num / base;
        }

        if (is_negative)
                buf[i++] = '-';

        buf[i] = '\0';

        strreverse(buf, i);

        return buf;
}
/* ------------------------------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------------------------------- */
static char *utoa(unsigned long long n, char *buf, int base)
{
        if (base < 2 || base > 36) {
                buf[0] = '\0';
                return buf;
        }

        char* p = buf;

        do {
                *p++ = "0123456789abcdefghijklmnopqrstuvwxyz"[n % base];
                n /= base;
        } while (n);

        *p = '\0';

        char* start = buf;
        char* end = p - 1;

        while (start < end) {
                char tmp = *start;
                *start++ = *end;
                *end-- = tmp;
        }

        return buf;
}
/* ------------------------------------------------------------------------------------------------- */

static char *ftoa(double n, char *buf)
{
        size_t DECIMALS = 6;

        char before_point_buf[65];
        char after_point_buf[65];
        char padded[DECIMALS + 1];

        bool negative = n < 0;
        double abs_n = negative ? -n : n;

        int64_t before_point = (int64_t)abs_n;
        char *before_point_str = itoa(before_point, before_point_buf, 10);

        double after_point_f = (abs_n - before_point) * pow(10, DECIMALS);
        int64_t after_point = (int64_t)after_point_f;
        char *after_point_str = itoa(after_point, after_point_buf, 10);

        size_t length = strlen(after_point_str);
        size_t padding = (length < DECIMALS) ? (DECIMALS - length) : 0;

        size_t i = 0;
        for(; i < padding; i++)
                padded[i] = '0';
        memcpy(padded + i, after_point_str, length < DECIMALS ? length : DECIMALS);
        padded[DECIMALS] = '\0';

        size_t pos = 0;
        if (negative)
                buf[pos++] = '-';

        size_t bp_length = strlen(before_point_str);
        memcpy(buf + pos, before_point_str, bp_length);
        pos += bp_length;

        buf[pos++] = '.';

        memcpy(buf + pos, padded, DECIMALS);
        pos += DECIMALS;

        buf[pos] = '\0';

        return buf;
}

static int format_to_buffer(char *restrict buffer, size_t n, const char *restrict format, va_list arg)
{
        char tmp[65];
        size_t pos = 0;

        while (*format) {
                if (*format != '%') {
                        if (buffer && pos + 1 < n)
                                buffer[pos] = *format;
                        pos++;
                        format++;
                        continue;
                }

                format++;
                if (*format == '\0')
                        break;

                int long_modifiers = 0;
                while (*format == 'l' && long_modifiers < 2) {
                        long_modifiers++;
                        format++;
                }

                const char *s = NULL;
                char ch_buf[2];

                switch (*format) {
                case FMT_CHAR: {
                        int ch = va_arg(arg, int);
                        ch_buf[0] = (char)ch;
                        ch_buf[1] = '\0';
                        s = ch_buf;
                        break;
                }
                case FMT_BINARY: {
                        unsigned long long v = (long_modifiers == 2) ? va_arg(arg, unsigned long long)
                                        : (long_modifiers == 1) ? va_arg(arg, unsigned long)
                                        : va_arg(arg, unsigned int);
                        s = utoa(v, tmp, 2);
                        break;
                }
                case FMT_OCTAL: {
                        unsigned long long v = (long_modifiers == 2) ? va_arg(arg, unsigned long long)
                                        : (long_modifiers == 1) ? va_arg(arg, unsigned long)
                                        : va_arg(arg, unsigned int);
                        s = utoa(v, tmp, 8);
                        break;
                }
                case FMT_DECIMAL: {
                        long long v = (long_modifiers == 2) ? va_arg(arg, long long)
                                : (long_modifiers == 1) ? va_arg(arg, long)
                                : va_arg(arg, int);
                        s = itoa(v, tmp, 10);
                        break;
                }
                case FMT_UNSIGNED: {
                        unsigned long long v = (long_modifiers == 2) ? va_arg(arg, unsigned long long)
                                        : (long_modifiers == 1) ? va_arg(arg, unsigned long)
                                        : va_arg(arg, unsigned int);
                        s = utoa(v, tmp, 10);
                        break;
                }
                case FMT_HEXADECIMAL: {
                        unsigned long long v = (long_modifiers == 2) ? va_arg(arg, unsigned long long)
                                        : (long_modifiers == 1) ? va_arg(arg, unsigned long)
                                        : va_arg(arg, unsigned int);
                        s = utoa(v, tmp, 16);
                        break;
                }
                case FMT_DOUBLE: {
                        double v = va_arg(arg, double);
                        s = ftoa(v, tmp);
                        break;
                }
                case FMT_STR: {
                        s = va_arg(arg, const char *);
                        break;
                }
                case '%': {
                        ch_buf[0] = '%';
                        ch_buf[1] = '\0';
                        s = ch_buf;
                        break;
                }
                default:
                        break;
                }

                if (s) {
                        while (*s) {
                                if (buffer && pos + 1 < n)
                                        buffer[pos] = *s;
                                pos++;
                                s++;
                        }
                }

                format++;
        }

        if (buffer && n > 0)
                buffer[pos < n ? pos : n - 1] = '\0';

        return (int)pos;
}

/* ------------------------------------------------------------------------------------------------- */
int vfprintf(FILE *stream, const char *restrict format, va_list arg)
{
        char buf[65];
        int res = 0;
        int long_modifiers = 0;

        while (*format) {
                if (*format != '%') {
                        res = write(stream->fd, &*format, sizeof(*format));
                        if (res < 0)
                                return res;
                        format++;
                        continue;
                }

                ++format;          // eat the '%'
                switch(*format) {
                case 'l': {
                        ++format;
                        if (*format == 'l') {
                                long_modifiers = 2;
                                ++format;
                        } else {
                                long_modifiers = 1;
                        }
                        break;
                }

                case FMT_CHAR: {
                        int ch = va_arg(arg, int);
                        res = fputc(ch, stream);
                        if (res < 0)
                                return res;
                        break;
                }

                case FMT_BINARY: {
                        size_t n = va_arg(arg, size_t);
                        char *s = utoa(n, buf, 2);
                        res = fprints(s, stream);
                        if (res < 0)
                                return res;
                        break;
                }

                case FMT_OCTAL: {
                        unsigned long long n = (long_modifiers == 2) ? va_arg(arg, unsigned long long)
                                        : (long_modifiers == 1) ? va_arg(arg, unsigned long)
                                        : va_arg(arg, unsigned int);
                        char *s = utoa(n, buf, 8);
                        res = fprints(s, stream);
                        if (res < 0)
                                return res;
                        break;
                }

                case FMT_DECIMAL: {
                        long long n = (long_modifiers == 2) ? va_arg(arg, long long)
                                : (long_modifiers == 1) ? va_arg(arg, long)
                                : va_arg(arg, int);
                        char *s = itoa(n, buf, 10);
                        res = fprints(s, stream);
                        if (res < 0)
                                return res;
                        break;
                }

                case FMT_UNSIGNED: {
                        unsigned long long n = (long_modifiers == 2) ? va_arg(arg, unsigned long long)
                                        : (long_modifiers == 1) ? va_arg(arg, unsigned long)
                                        : va_arg(arg, unsigned int);
                        char *s = utoa(n, buf, 10);
                        res = fprints(s, stream);
                        if (res < 0)
                                return res;
                        break;
                }

                case FMT_HEXADECIMAL: {
                        unsigned long long n = (long_modifiers == 2) ? va_arg(arg, unsigned long long)
                                        : (long_modifiers == 1) ? va_arg(arg, unsigned long)
                                        : va_arg(arg, unsigned int);
                        char *s = utoa(n, buf, 16);
                        res = fprints(s, stream);
                        if (res < 0)
                                return res;
                        break;
                }

                case FMT_DOUBLE: {
                        double n = va_arg(arg, double);
                        char *s = ftoa(n, buf);
                        res = fprints(s, stream);
                        if (res < 0)
                                return res;
                        break;
                }

                case FMT_STR: {
                        const char *s = va_arg(arg, const char *);
                        res = fprints(s, stream);
                        if (res < 0)
                                return res;
                        break;
                }

                default:
                        break;
                }

                format++;
        }

        return 0;
}
/* ------------------------------------------------------------------------------------------------- */

int vsprintf(char *restrict buffer, const char *restrict format, va_list vlist)
{
        return format_to_buffer(buffer, (size_t)-1, format, vlist);
}

int vsnprintf(char *restrict buffer, size_t bufsz, const char *restrict format, va_list vlist)
{
        return format_to_buffer(buffer, bufsz, format, vlist);
}

int sprintf(char *restrict buffer, const char *restrict format, ...)
{
        va_list args;
        va_start(args, format);
        int res = vsprintf(buffer, format, args);
        va_end(args);
        return res;
}

int snprintf(char *restrict buffer, size_t bufsz, const char *restrict format, ...)
{
        va_list args;
        va_start(args, format);
        int res = vsnprintf(buffer, bufsz, format, args);
        va_end(args);
        return res;
}

/* ------------------------------------------------------------------------------------------------- */
int vprintf(const char *restrict format, va_list arg)
{
        int res = vfprintf(stdout, format, arg);
        return res;
}
/* ------------------------------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------------------------------- */
int fprintf(FILE *stream, const char *restrict format, ...)
{
        va_list args;
        va_start(args, format);
        int res = vfprintf(stream, format, args);
        va_end(args);
        return res;
}
/* ------------------------------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------------------------------- */
int printf(const char *restrict format, ...)
{
        va_list args;
        va_start(args, format);
        int res = vprintf(format, args);
        va_end(args);
        return res;
}
/* ------------------------------------------------------------------------------------------------- */

END_DECLS