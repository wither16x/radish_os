#!/bin/bash

set -e

mkdir -p .build_initrd

tar --format=ustar -cf .build_initrd/initrd.tar -C initrd . 