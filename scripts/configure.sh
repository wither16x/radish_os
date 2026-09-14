#!/bin/bash

set -e

echo "RadishOS Configuration Program"
echo "=========================================================="
echo "Components:"
echo "k - Kernel"
echo "q - Quit"
read -p "> " component

if [[ $component == "k" ]]; then
        opt_cc=${opt_cc:-"x86_64-radishos-clang"}
        opt_cxx=${opt_cxx:-"x86_64-radishos-clang++"}
        opt_nasm=${opt_nasm:-"nasm"}
        opt_linker=${opt_linker:-"x86_64-radishos-clang++"}
        opt_build_mode=${opt_build_mode:-"release"}
        config_file="kernel/config.mk"

        echo "-------------------- Kernel configuration --------------------"
        echo "Leave blank for default option."
        
        read -p "C compiler [$opt_cc]: " input
        opt_cc=${input:-$opt_cc}
        read -p "C++ compiler [$opt_cxx]: " input
        opt_cxx=${input:-$opt_cxx}
        read -p "NASM [$opt_nasm]: " input
        opt_nasm=${input:-$opt_nasm}
        read -p "Linker [$opt_linker]: " input
        opt_linker=${input:-$opt_linker}
        read -p "Build mode (dev/test/release) [$opt_build_mode]: " input
        opt_build_mode=${input:-$opt_build_mode}

        echo "-> Generating configuration..."
        echo "BUILD_MODE = $opt_build_mode" > $config_file
        echo "ifeq (\$(filter \$(BUILD_MODE),dev test release),)" >> $config_file
        echo "\$(error Build mode '\$(BUILD_MODE)' not supported)" >> $config_file
        echo "endif" >> $config_file
        echo "C_COMPILER = $opt_cc" >> $config_file
        echo "CXX_COMPILER = $opt_cxx" >> $config_file
        echo "NASM = $opt_nasm" >> $config_file
        echo "LINKER = $opt_linker" >> $config_file
        echo "TEST_DIR = test" >> $config_file
        echo "INCLUDE_DIR = include" >> $config_file
        echo "COMMON_FLAGS = -ffreestanding -fno-stack-protector -fno-stack-check -fno-lto -fno-pic -fno-pie -mno-red-zone -Wall -Wextra -nostdlib -nostdinc -mcmodel=kernel" >> $config_file
        echo "ifeq (\$(BUILD_MODE),dev)" >> $config_file
        echo "COMMON_FLAGS += -g" >> $config_file
        echo "endif" >> $config_file
        echo "ifeq (\$(BUILD_MODE),test)" >> $config_file
        echo "COMMON_FLAGS += -g -I../\$(TEST_DIR) -DKIWI_BUILD_MODE_TEST" >> $config_file
        echo "endif" >> $config_file
        echo "C_FLAGS = \$(COMMON_FLAGS) -std=c23 -fno-builtin -fvisibility=hidden" >> $config_file
        echo "CXX_FLAGS	= \$(COMMON_FLAGS) -std=c++23 -fno-exceptions -fno-rtti -fno-threadsafe-statics -fno-use-cxa-atexit -fno-unwind-tables -fno-asynchronous-unwind-tables -fno-builtin -fvisibility=hidden -fno-sized-deallocation" >> $config_file
        echo "NASM_FLAGS = -f elf64" >> $config_file
        echo "LINKER_FLAGS = -static -nostdlib -Wl,--build-id=none -z max-page-size=0x1000 -T kernel.lds -fuse-ld=lld" >> $config_file
        echo "-> Wrote configuration to $config_file"
elif [[ $component == "q" ]]; then
        exit 0
else
        echo "Invalid component: '$component'."
fi