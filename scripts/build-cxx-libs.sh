#!/bin/bash

# This script will install libc++abi and libunwind
# $1 = sysroot

# clang > 19.x is required

mkdir -p .build_cxx_libs

git clone https://github.com/llvm/llvm-project \
        .build_cxx_libs/llvm-project/ \
        --depth=1 --branch=main

set -e

cd .build_cxx_libs/
# Gonna remove useless flags later
cmake -G Ninja -S llvm-project/runtimes -B build \
        -DLLVM_ENABLE_RUNTIMES="libunwind;libcxxabi;libcxx;compiler-rt" \
        -DCMAKE_C_COMPILER="clang" \
        -DCMAKE_CXX_COMPILER="clang++" \
        -DCMAKE_C_COMPILER_TARGET="x86_64-unknown-none" \
        -DCMAKE_CXX_COMPILER_TARGET="x86_64-unknown-none" \
        -DCMAKE_SYSROOT="$1" \
        -DCMAKE_INSTALL_PREFIX="$1" \
        -DLIBCXXABI_USE_LLVM_UNWINDER=ON \
        -DLIBCXX_CXX_ABI=libcxxabi \
        -DLIBCXX_ENABLE_THREADS=OFF \
        -DLIBCXXABI_ENABLE_THREADS=OFF \
        -DLIBCXX_INCLUDE_TESTS=OFF \
        -DLIBCXXABI_INCLUDE_TESTS=OFF \
        -DLIBUNWIND_INCLUDE_TESTS=OFF \
        -DCMAKE_SYSTEM_NAME=Generic \
        -DLIBUNWIND_ENABLE_SHARED=OFF \
        -DLIBUNWIND_ENABLE_THREADS=OFF \
        -DLIBUNWIND_IS_BAREMETAL=ON \
        -DLIBUNWIND_ENABLE_CROSS_UNWINDING=OFF \
        -DCXX_SUPPORTS_WERROR_EQ_RETURN_TYPE_FLAG=0 \
        -DCMAKE_C_FLAGS="-Wno-error=return-type -D_LIBCPP_DISABLE_VISIBILITY_ANNOTATIONS" \
        -DCMAKE_CXX_FLAGS="-Wno-error=return-type -D_LIBCPP_DISABLE_VISIBILITY_ANNOTATIONS" \
        -DLIBCXX_ENABLE_FILESYSTEM=OFF \
        -DLIBCXX_ENABLE_WIDE_CHARACTERS=OFF \
        -DLIBCXX_ENABLE_LOCALIZATION=OFF \
        -DCMAKE_CXX_FLAGS="-Wno-error=return-type -D_LIBCPP_DISABLE_VISIBILITY_ANNOTATIONS -D_LIBCPP_PROVIDES_DEFAULT_RUNE_TABLE" \
        -DCMAKE_C_FLAGS="-Wno-error=return-type -D_LIBCPP_DISABLE_VISIBILITY_ANNOTATIONS -D_LIBCPP_PROVIDES_DEFAULT_RUNE_TABLE" \
        -DLIBCXX_ENABLE_MONOTONIC_CLOCK=OFF \
        -DCOMPILER_RT_BUILD_BUILTINS=ON \
        -DCOMPILER_RT_BUILD_SANITIZERS=OFF \
        -DCOMPILER_RT_BUILD_XRAY=OFF \
        -DCOMPILER_RT_BUILD_LIBFUZZER=OFF \
        -DCOMPILER_RT_BUILD_PROFILE=OFF \
        -DCOMPILER_RT_BUILD_MEMPROF=OFF \
        -DCOMPILER_RT_BUILD_ORC=OFF \
        -DCOMPILER_RT_BUILD_GWP_ASAN=OFF \
        -DCOMPILER_RT_BAREMETAL_BUILD=ON

ninja -C build install-compiler-rt install-cxx install-cxxabi install-unwind