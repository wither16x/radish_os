#!/bin/bash

set -e

mkdir -p initrd/bin
nasm -f bin userspace/test.asm -o initrd/bin/test.bin

mkdir -p .build_initrd
tar --format=ustar -cf .build_initrd/initrd.tar -C initrd .