#!/bin/bash

set -e

make -C kernel clean
make -C kernel

if [[ "$1" == "--symbols" ]]; then
        chmod +x scripts/symbols.sh
        ./scripts/symbols.sh

        # recompile the kernel (see scripts/symbols.sh) for
        # nore info
        make -C kernel clean
        make -C kernel
fi

chmod +x scripts/mkinitrd.sh
./scripts/mkinitrd.sh

chmod +x scripts/mkiso.sh
./scripts/mkiso.sh