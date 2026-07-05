#!/bin/bash

set -e

CLANG="x86_64-radishos-clang"
CLANGXX="x86_64-radishos-clang++"

# sysroot (assuming libc has already been built)
mkdir -p sysroot
mkdir -p sysroot/usr

mkdir -p sysroot/usr/include
cp -v libc/include/stdio.h sysroot/usr/include/stdio.h

mkdir -p sysroot/usr/lib
cp -v libc/build/crt0.o sysroot/usr/lib/crt0.o
cp -v libc/build/libc.a sysroot/usr/lib/libc.a

# clang
echo -e "#!/bin/bash"                                           > $CLANG
echo -e "clang --target=\"x86_64-unknown-none\" \\"             >> $CLANG
echo -e "\t-D__radishos__ \\"                                   >> $CLANG     
echo -e "\t-m64 \\"                                             >> $CLANG
echo -e "\t--sysroot="$1"/sysroot \\"                                >> $CLANG
echo -e "\t\$@"                                                 >> $CLANG
chmod +x $CLANG
echo "Generated $CLANG"

# clang++
echo -e "#!/bin/bash"                                           > $CLANGXX
echo -e "clang++ --target=\"x86_64-unknown-none\" \\"           >> $CLANGXX
echo -e "\t-D__radishos__ \\"                                   >> $CLANGXX
echo -e "\t-m64 \\"                                             >> $CLANGXX
echo -e "\t--sysroot="$1"/sysroot \\"                                >> $CLANGXX
echo -e "\t\$@"                                                   >> $CLANGXX
chmod +x $CLANGXX
echo "Generated $CLANGXX"