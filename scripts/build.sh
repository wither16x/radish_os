#!/bin/bash

set -e

chmod +x scripts/symbols.sh
./scripts/symbols.sh

make -C kernel clean
make -C kernel

chmod +x scripts/mkinitrd.sh
./scripts/mkinitrd.sh

chmod +x scripts/mkiso.sh
./scripts/mkiso.sh