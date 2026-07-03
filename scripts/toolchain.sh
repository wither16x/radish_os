#!/bin/bash

set -e

DIR=".build_toolchain"

mkdir -p $DIR

# Clone the directories
if [ -d "$DIR/binutils-gdb" ]; then
        echo "$DIR/binutils-gdb already exists, assuming it contains the right sources"
else
        git clone git://sourceware.org/git/binutils-gdb.git "$DIR/binutils-gdb" --depth=1
        cd "$DIR/binutils-gdb"
        git checkout 794f538c
        git am patches/binutils-gdb/*.patch
        cd -
fi

if [ -d "$DIR/gcc" ]; then
        echo "$DIR/gcc already exists, assuming it contains the right sources"
else
        git clone https://gcc.gnu.org/git/gcc.git "$DIR/gcc" --depth=1
        cd "$DIR/gcc"
        git checkout 158250754
        git am patches/gcc/*.patch
        cd -
fi

# Prepare build
export PREFIX="$HOME/opt/cross"
export TARGET="x86_64-radishos"
export PATH="$PREFIX/bin:$PATH"

# Create source directory
mkdir -p $DIR/src
cd $DIR/src

# Build binutils
if [ -d "build_binutils" ]; then
       echo "It looks like binutils has already been built. If not, then please remove the build directory."
else
        mkdir -p build_binutils
        cd build_binutils
        ../../binutils-gdb/configure --target="$TARGET" --prefix="$PREFIX" \
                --with-sysroot --disable-nls --disable-werror --enable-default-execstack=no
        make
        make install
        cd ..
fi

# Build GCC
if [ -d "build_gcc" ]; then
        echo "It looks like gcc has already been built. If not, then please remove the build directory."
else
        which -- $TARGET-as || echo "$TARGET-as is not in PATH"

        mkdir -p build_gcc
        cd build_gcc

        ../../gcc/configure --target="$TARGET" --prefix="$PREFIX" --disable-nls \
                --enable-languages=c,c++ --without-headers --enable-initfini-array \
                --disable-hosted-libstdcxx --disable-multilib

        make all-gcc
        make all-target-libgcc
        make all-target-libstdc++-v3
        make install-gcc
        make install-target-libgcc
        make install-target-libstdc++-v3
fi

# Use the new compiler by simply invoking $TARGET-program.
# The line below should be executed by the user.
# export PATH="$HOME/opt/cross/bin:$PATH"