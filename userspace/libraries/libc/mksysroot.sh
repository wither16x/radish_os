#!/bin/bash

set -e

mkdir -p sysroot
mkdir -p sysroot/usr

cp -v -r include/* sysroot/usr/include/

mkdir -p sysroot/usr/lib
cp -v build/crt0.o sysroot/usr/lib/crt0.o
cp -v build/crti.o sysroot/usr/lib/crti.o
cp -v build/crtn.o sysroot/usr/lib/crtn.o
cp -v build/libc.a sysroot/usr/lib/libc.a