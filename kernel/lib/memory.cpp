#include <lib/memory.hpp>

namespace Kiwi::Lib
{
        extern "C"
        {
                void *memcpy(void *__restrict dest, const void *__restrict src, usize n)
                {
                        u8 *__restrict pdest = static_cast<u8 *__restrict>(dest);
                        const u8 *__restrict psrc = static_cast<const u8 *__restrict>(src);

                        for (usize i = 0; i < n; i++)
                                pdest[i] = psrc[i];

                        return dest;
                }

                void *memset(void *s, int c, usize n)
                {
                        u8 *p = static_cast<u8 *>(s);

                        for (usize i = 0; i < n; i++)
                                p[i] = static_cast<u8>(c);

                        return s;
                }

                void *memmove(void *dest, const void *src, usize n)
                {
                        u8 *pdest = static_cast<u8 *>(dest);
                        const u8 *psrc = static_cast<const u8 *>(src);

                        if (reinterpret_cast<uptr>(src) > reinterpret_cast<uptr>(dest)) {
                                for (usize i = 0; i < n; i++)
                                        pdest[i] = psrc[i];
                        } else if (reinterpret_cast<uptr>(src) < reinterpret_cast<uptr>(dest)) {
                                for (usize i = n; i > 0; i--)
                                        pdest[i-1] = psrc[i-1];
                        }

                        return dest;
                }

                int memcmp(const void *s1, const void *s2, usize n)
                {
                        const u8 *p1 = static_cast<const u8 *>(s1);
                        const u8 *p2 = static_cast<const u8 *>(s2);

                        for (usize i = 0; i < n; i++) {
                                if (p1[i] != p2[i])
                                        return p1[i] < p2[i] ? -1 : 1;
                        }

                        return 0;
                }
        } // extern "C"

        int strcmp(const char *s1, const char *s2)
        {
                while (*s1 and *s2) {
                        if (*s1 != *s2)
                                break;

                        s1++;
                        s2++;
                }

                return (*s1 - '0') - (*s2 - '0');
        }

        int strncmp(const char *s1, const char *s2, usize length)
        {
                while (length && *s1 && (*s1 == *s2)) {
                        s1++;
                        s2++;
                        --length;
                }

                return (*s1 - '0') - (*s2 - '0');
        }

        usize strlen(const char *s)
        {
                usize i;
                for (i = 0; *s; s++)
                        i++;
                return i;
        }

        void strcpy(const char *src, char *dest)
        {
                int i;

                for (i = 0; src[i]; i++)
                        dest[i] = src[i];

                dest[i] = '\0';
        }

        bool strstartswith(const char *s1, const char *s2)
        {
                return strncmp(s1, s2, strlen(s2)) == 0;
        }

        char *strcat(char *dest, const char *src)
        {
                usize i, j;

                for (i = 0; dest[i] != '\0'; i++);
                for (j = 0; src[j] != '\0'; j++)
                        dest[i + j] = src[j];
                dest[i + j] = '\0';
                
                return dest;
        }
} // namespace Kiwi::Lib