#!/bin/bash

set -e

mkdir -p initrd/bin
mkdir -p .build_initrd

cp -v userspace/hello/bin/hello initrd/bin/hello

tar --format=ustar -cf .build_initrd/initrd.tar -C initrd .