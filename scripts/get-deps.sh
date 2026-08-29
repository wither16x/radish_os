#!/bin/bash

DEPENDENCIES_DIR=".dependencies"

# $1: link to the repo
# $2: output dir
# $3: branch
function clone_repo {
        git clone "$1" "$2" --branch="$3" --depth=1
}

mkdir -p "$DEPENDENCIES_DIR"

# clone repos
clone_repo https://github.com/Limine-Bootloader/limine-protocol \
           "$DEPENDENCIES_DIR/limine-protocol" \
           trunk

clone_repo https://github.com/Limine-Bootloader/Limine \
           "$DEPENDENCIES_DIR/Limine" \
           v11.x-binary

clone_repo https://github.com/wither16x/tomato-libc \
        "$DEPENDENCIES_DIR/tomato-libc" \
        master

clone_repo https://github.com/wither16x/tarwi \
        "$DEPENDENCIES_DIR/tarwi" \
        master

clone_repo https://github.com/wither16x/cash \
        "$DEPENDENCIES_DIR"/cash \
        master

# build stuff
make -C "$DEPENDENCIES_DIR/Limine"
make -C "$DEPENDENCIES_DIR/tomato-libc"
make -C "$DEPENDENCIES_DIR/cash"

# copy stuff
cp -v "$DEPENDENCIES_DIR/limine-protocol/include/limine.h" \
      kernel/include/limine.h

cp -v "$DEPENDENCIES_DIR/tarwi/Include/Tarwi.hpp" \
        kernel/test/Tarwi/Tarwi.hpp