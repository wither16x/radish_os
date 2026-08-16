#!/bin/bash

function clone_repo {
        git clone "$1" "$2" --depth=1 --branch="$3"
}

clone_repo https://github.com/codeplea/tinyexpr \
        ./ports/origins/tinyexpr \
        master
cd ports/origins/tinyexpr
git apply ../../tinyexpr/0001-Ported-to-RadishOS.patch
cd -