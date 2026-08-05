#!/bin/bash

set -e

function clone_repo {
        git clone "$1" "$2" --depth=1 --branch="$3"
}

clone_repo https://github.com/codeplea/tinyexpr \
        ./ports/origins/tinyexpr \
        master