#!/bin/bash

set -e

# the prefix must contains:
# usr/
# usr/include/
# usr/lib/
PREFIX="$1"

mkdir -p "$PREFIX"/usr/include/melon/

cp -v -r include/* "$PREFIX"/usr/include/melon/
cp -v build/libmelon.a "$PREFIX"/usr/lib/libmelon.a