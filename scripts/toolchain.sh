#!/bin/bash

set -e

CLANG="x86_64-radishos-clang"
CLANGXX="x86_64-radishos-clang++"

# clang
echo -e "#!/bin/bash"                                           > $CLANG
echo -e "clang --target=\"x86_64-unknown-none\" \\"             >> $CLANG
echo -e "\t-D__radishos__ \\"                                   >> $CLANG     
echo -e "\t-m64 \\"                                             >> $CLANG
echo -e "\t--sysroot="$1" \\"                                >> $CLANG
echo -e "\t\$@"                                                 >> $CLANG
chmod +x $CLANG
echo "Generated $CLANG"

# clang++
echo -e "#!/bin/bash"                                           > $CLANGXX
echo -e "clang++ --target=\"x86_64-unknown-none\" \\"           >> $CLANGXX
echo -e "\t-D__radishos__ \\"                                   >> $CLANGXX
echo -e "\t-m64 \\"                                             >> $CLANGXX
echo -e "\t--sysroot="$1" \\"                                >> $CLANGXX
echo -e "\t-I"$1"/usr/include/c++/v1 \\"                >> $CLANGXX
echo -e "\t\$@"                                                   >> $CLANGXX
chmod +x $CLANGXX
echo "Generated $CLANGXX"

# prefixes
echo -e "TOOL_PREFIX := x86_64-radishos"                        > prefixes.mk
echo -e "LIBC_PREFIX := \"$1\"/usr/lib"                                 >> prefixes.mk
echo "Generated prefixes.mk"