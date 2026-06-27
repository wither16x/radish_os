#!/bin/bash

#set -e

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

# build stuff
make -C "$DEPENDENCIES_DIR/Limine"

# copy stuff
cp -v "$DEPENDENCIES_DIR/limine-protocol/include/limine.h" \
      kernel/include/limine.h