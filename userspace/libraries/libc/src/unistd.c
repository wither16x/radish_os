#include "unistd.h"
#include "__utils/decls.h"
#include <stddef.h>

#define GET_SYS_RETVAL(var)     __asm__ volatile ("" : "=a"(var))
#define START_SYSCALL(n)        __asm__ volatile ( "int $0x80" :: "a"(n),
#define END_SYSCALL             );
#define SYSCALL_NOPARAM(n)      __asm__ volatile ("int $0x80" :: "a"(n));
#define RBX(x)                  "b"(x)
#define RCX(x)                  "c"(x)
#define RDX(x)                  "d"(x)
#define RDI(x)                  "D"(x)

enum syscall_type
{
        SYS_WRITE,
        SYS_READ,
        SYS_EXEC,
        SYS_FORK,
        SYS_EXIT,
        SYS_GETPID,
        SYS_WAIT,
        SYS_OPEN,
        SYS_CLOSE,
        SYS_LASTPG,
        SYS_GETCPUTIME,
        SYS_RM,
        SYS_SEEK
};

BEGIN_DECLS

int write(int fd, const void *buf, size_t n)
{
        int ret;
        START_SYSCALL(SYS_WRITE)
                RBX(fd),
                RCX(buf),
                RDX(n)
        END_SYSCALL
        GET_SYS_RETVAL(ret);
        return ret;
}

int read(int fd, void *buf, size_t n)
{
        int ret;
        START_SYSCALL(SYS_READ)
                RBX(fd),
                RCX(buf),
                RDX(n)
        END_SYSCALL
        GET_SYS_RETVAL(ret);
        return ret;
}

int exec(const char *file, int argc, char **argv, char **envp)
{
        int ret;
        START_SYSCALL(SYS_EXEC)
                RBX(file),
                RCX(argc),
                RDX(argv),
                RDI(envp)
        END_SYSCALL
        GET_SYS_RETVAL(ret);
        return ret;
}

pid_t fork(void)
{
        int ret;
        SYSCALL_NOPARAM(SYS_FORK);
        GET_SYS_RETVAL(ret);
        return ret;
}

int exit(void)
{
        int ret;
        SYSCALL_NOPARAM(SYS_EXIT);
        GET_SYS_RETVAL(ret);
        return ret;
}

int getpid(void)
{
        int ret;
        SYSCALL_NOPARAM(SYS_GETPID);
        GET_SYS_RETVAL(ret);
        return ret;
}

int wait(void)
{
        int ret;
        SYSCALL_NOPARAM(SYS_WAIT);
        GET_SYS_RETVAL(ret);
        return ret;
}

int open(const char *path, int flags, ...)
{
        (void)flags;
        int ret;
        START_SYSCALL(SYS_OPEN)
                RBX(path)
        END_SYSCALL
        GET_SYS_RETVAL(ret);
        return ret;
}

int close(int fd)
{
        int ret;
        START_SYSCALL(SYS_CLOSE)
                RBX(fd)
        END_SYSCALL
        GET_SYS_RETVAL(ret);
        return ret;
}

void *lastpg(int pages)
{
        void *ret;
        START_SYSCALL(SYS_LASTPG)
                RBX(pages)
        END_SYSCALL
        GET_SYS_RETVAL(ret);
        return ret;
}

int64_t getcputime(void)
{
        int64_t ret;
        SYSCALL_NOPARAM(SYS_GETCPUTIME);
        GET_SYS_RETVAL(ret);
        return ret;
}

int rm(const char *path)
{
        int ret;
        START_SYSCALL(SYS_RM)
                RBX(path)
        END_SYSCALL
        GET_SYS_RETVAL(ret);
        return ret;
}

size_t seek(int fd, size_t pos, size_t whence)
{
        size_t ret;
        START_SYSCALL(SYS_SEEK)
                RBX(fd),
                RCX(pos),
                RDX(whence)
        END_SYSCALL
        GET_SYS_RETVAL(ret);
        return ret;
}

END_DECLS