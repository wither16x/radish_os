#!/bin/bash

set -e

make -C kernel clean
make -C kernel

chmod +x scripts/mkusr.sh
./scripts/mkusr.sh

chmod +x scripts/mkinitrd.sh
./scripts/mkinitrd.sh

chmod +x scripts/mkimage.sh
./scripts/mkimage.sh --iso