#!/bin/bash

set -e

mkdir -p initrd/bin
nasm -f elf64 userspace/test.asm -o userspace/test.o
ld -Ttext 0x400000 userspace/test.o -o initrd/bin/test

mkdir -p .build_initrd
tar --format=ustar -cf .build_initrd/initrd.tar -C initrd .