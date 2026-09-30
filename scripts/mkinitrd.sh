#!/bin/bash

set -e

mkdir -p initrd/bin
mkdir -p .build_initrd

cp -v userspace/init/bin/init initrd/bin/init
cp -v userspace/cash/bin/cash initrd/bin/cash

cp -v userspace/utils/echo/bin/echo initrd/bin/echo

mkdir -p initrd/bin/tinyexpr
cp -v ports/origins/tinyexpr/repl initrd/bin/tinyexpr/repl
cp -v ports/origins/tinyexpr/example initrd/bin/tinyexpr/example
cp -v ports/origins/tinyexpr/example2 initrd/bin/tinyexpr/example2
cp -v ports/origins/tinyexpr/example3 initrd/bin/tinyexpr/example3

tar --format=ustar -cf .build_initrd/initrd.tar -C initrd .