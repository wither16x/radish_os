#!/bin/bash

# This script will install libc++, libc++abi and libunwind
# $1 = sysroot

mkdir -p .build_cxx_libs

git clone https://github.com/llvm/llvm-project \
        .build_cxx_libs/llvm-project/ \
        --depth=1 --branch=main

set -e

cd .build_cxx_libs/
cmake -G Ninja -S llvm-project/runtimes -B build \
        -DLLVM_ENABLE_RUNTIMES="libunwind;libcxxabi" \
        -DCMAKE_C_COMPILER="clang" \
        -DCMAKE_CXX_COMPILER="clang++" \
        -DCMAKE_C_COMPILER_TARGET="x86_64-unknown-none" \
        -DCMAKE_CXX_COMPILER_TARGET="x86_64-unknown-none" \
        -DCMAKE_SYSROOT="$1" \
        -DCMAKE_INSTALL_PREFIX="$1" \
        -DLIBCXXABI_USE_LLVM_UNWINDER=ON \
        -DLIBCXX_CXX_ABI=libcxxabi \
        -DLIBCXXABI_ENABLE_THREADS=OFF \
        -DLIBCXXABI_INCLUDE_TESTS=OFF \
        -DLIBUNWIND_INCLUDE_TESTS=OFF \
        -DCMAKE_SYSTEM_NAME=Generic \
        -DLIBUNWIND_ENABLE_SHARED=OFF \
        -DLIBUNWIND_ENABLE_THREADS=OFF \
        -DLIBUNWIND_IS_BAREMETAL=ON \
        -DLIBUNWIND_ENABLE_CROSS_UNWINDING=OFF \
        -DCMAKE_C_FLAGS="-Wno-error=return-type" \
        -DCMAKE_CXX_FLAGS="-Wno-error=return-type" \
        -DCXX_SUPPORTS_WERROR_EQ_RETURN_TYPE_FLAG=0

ninja -C build install-cxxabi install-unwind