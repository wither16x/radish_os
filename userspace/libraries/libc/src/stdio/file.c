#include "__utils/decls.h"
#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include "string.h"

BEGIN_DECLS

int fileno(FILE *stream)
{
        return stream->fd;
}

FILE *fdopen(int fd, const char *modes)
{
        (void)modes;
        FILE *f = (FILE *)malloc(sizeof(*f));
        if (!f)
                return NULL;
        f->fd = fd;
        f->buf = (char *)malloc(sizeof(*f->buf));

        return f;
}

FILE *fopen(const char *restrict path, const char *modes)
{
        int fd = open(path, 0);
        FILE *f = fdopen(fd, modes);
        return f;
}

int fclose(FILE *stream)
{
        if (!stream)
                return -1;
        int result = close(fileno(stream));
        free(stream);
        return result;
}

size_t fwrite(const void *restrict buffer, size_t size, size_t count, FILE *restrict stream)
{
        (void)size;
        memcpy(stream->buf, buffer, size * count);
        return 0;
}

int fputc(int c, FILE *stream)
{
        return write(fileno(stream), (const void *)&c, 1);
}

int fgetc(FILE *stream)
{
        int c = 0;
        if (read(fileno(stream), &c, 1) != 0)
                return -1;
        return c;
}

char *fgets(char *restrict str, int count, FILE *stream)
{
        int ch = 0;
        int i = 0;

        if (count <= 0)
                return str;

        while (i < count - 1) {
                ch = fgetc(stream);

                if (ch < 0)
                        break;

                str[i++] = ch;
                
                if (ch == '\n')
                        break;
        }
        str[i] = '\0';

        if (i > 0)
                return str;
        return NULL;
}

int fflush(FILE *stream)
{
        (void)stream;
        return -1;
}

int remove(const char *pathname)
{
        int result = rm(pathname);
        return result;
}

int isatty(int fd)
{
        (void)fd;
        return -1;
}

int feof(FILE *stream)
{
        (void)stream;
        return -1;
}

int ferror(FILE *stream)
{
        (void)stream;
        return -1;
}

int fputs(const char *restrict s, FILE *restrict stream)
{
        return write(fileno(stream), (const void *)s, strlen(s));
}

int fseek(FILE *stream, long offset, int whence)
{
        return seek(fileno(stream), offset, whence);
}

END_DECLS