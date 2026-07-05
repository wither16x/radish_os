#!/bin/bash

set -e

CLANG="x86_64-radishos-clang"
CLANGXX="x86_64-radishos-clang++"

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