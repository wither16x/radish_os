#!/bin/bash

set -e

mkdir -p initrd/bin
mkdir -p .build_initrd

cp -v userspace/hello/bin/hello initrd/bin/hello
cp -v userspace/shell/bin/shell initrd/bin/shell

tar --format=ustar -cf .build_initrd/initrd.tar -C initrd .