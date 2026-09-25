#pragma once

#include <stdint.h>
#include <stddef.h>
#include "__utils/decls.h"

typedef size_t          pid_t;

BEGIN_DECLS

int write(int fd, const void *buf, size_t n);
int read(int fd, void *buf, size_t n);
int exec(const char *file, int argc, char **argv, char **envp);
pid_t fork(void);
int exit(void);
int getpid(void);
int wait(void);
int open(const char *path, int flags, ...);
int close(int fd);
void *lastpg(int pages);
int64_t getcputime(void);
int rm(const char *path);
size_t seek(int fd, size_t pos, size_t whence);

END_DECLS