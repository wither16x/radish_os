#!/bin/bash

set -e

CLANG="x86_64-radishos-clang"
CLANGXX="x86_64-radishos-clang++"

# checks
if [ -f $CLANG ]; then
        if [ "$1" == "--rebuild" ]; then
                rm $CLANG
        else
                echo "$CLANG has already been generated"
                exit 1
        fi
fi

if [ -f $CLANGXX ]; then
        if [ "$1" == "--rebuild" ]; then
                rm $CLANGXX
        else
                echo "$CLANGXX has already been generated"
                exit 1
        fi
fi

# clang
echo -e "#!/bin/bash"                                           > $CLANG
echo -e "clang --target=\"x86_64-unknown-none\" \\"             >> $CLANG
echo -e "\t-D__radishos__ \\"                                   >> $CLANG     
echo -e "\t-m64 \\"                                             >> $CLANG
echo -e "\t\$@"                                                 >> $CLANG
chmod +x $CLANG
echo "Generated $CLANG"

# clang++
echo -e "#!/bin/bash"                                           > $CLANGXX
echo -e "clang++ --target=\"x86_64-unknown-none\" \\"           >> $CLANGXX
echo -e "\t-D__radishos__ \\"                                   >> $CLANGXX
echo -e "\t-m64 \\"                                             >> $CLANGXX
echo -e "\$@"                                                   >> $CLANGXX
chmod +x $CLANGXX
echo "Generated $CLANGXX"