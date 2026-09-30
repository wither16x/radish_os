#!/bin/bash

set -e

cmake -G Ninja -S ports/origins/llvm-project/llvm -B build-clang \
        -DCMAKE_BUILD_TYPE=Release \
        -DLLVM_ENABLE_PROJECTS="clang;lld" \
        -DLLVM_TARGETS_TO_BUILD=X86 \
        -DCMAKE_INSTALL_PREFIX="$1/radish-toolchain" \
        -DDEFAULT_SYSROOT="$1" \
        -DCLANG_DEFAULT_CXX_STDLIB=libc++ \
        -DCLANG_DEFAULT_RTLIB=compiler-rt \
        -DCLANG_DEFAULT_UNWINDLIB=libunwind \
        -DCLANG_DEFAULT_LINKER=lld \
        -DLLVM_PARALLEL_LINK_JOBS=2 \
        -DLLVM_INCLUDE_TESTS=OFF \
        -DLLVM_INCLUDE_EXAMPLES=OFF \
        -DLLVM_INCLUDE_BENCHMARKS=OFF \
        -DCLANG_INCLUDE_TESTS=OFF \
        -DLLVM_ENABLE_ASSERTIONS=OFF \
        -DLLVM_BUILD_TESTS=OFF \
        -DCMAKE_INTERPROCEDURAL_OPTIMIZATION=OFF

ninja -C build-clang install