#!/bin/bash

set -e

mkdir -p initrd/bin
mkdir -p .build_initrd

cp -v userspace/hello/bin/hello initrd/bin/hello
cp -v userspace/shell/bin/shell initrd/bin/shell
cp -v userspace/init/bin/init initrd/bin/init
cp -v userspace/alloc/bin/alloc initrd/bin/alloc
cp -v userspace/cxx-test/bin/cxx_test initrd/bin/cxx_test

mkdir -p initrd/bin/tinyexpr
cp -v ports/origins/tinyexpr/repl initrd/bin/tinyexpr/repl
cp -v ports/origins/tinyexpr/example initrd/bin/tinyexpr/example
cp -v ports/origins/tinyexpr/example2 initrd/bin/tinyexpr/example2
cp -v ports/origins/tinyexpr/example3 initrd/bin/tinyexpr/example3

tar --format=ustar -cf .build_initrd/initrd.tar -C initrd .