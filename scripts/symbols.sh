#!/bin/bash

KERNEL_MAP=".symbols_data/kernel.map"
SYM_HDR="kernel/kernel_sym_entries.h"

mkdir -p .symbols_data
echo "Created directory: .symbols_data"

nm -n kernel/bin/kernel.elf > .symbols_data/kernel.map
echo "Generated .symbols_data/kernel.map"
cat $KERNEL_MAP | awk '{ printf("    { 0x%s, \"%s\" },\n", $1, $3); }' > ".symbols_data/kernel_sym_entries.txt"
echo "Generated .symbols_data/kernel_sym_entries.txt"

echo '#include "symbols.h"' > $SYM_HDR
echo '#include "stddef.h"' >> $SYM_HDR
echo 'static struct symbol kernel_symbols[] = {' >> $SYM_HDR
cat .symbols_data/kernel_sym_entries.txt >> $SYM_HDR
echo '};' >> $SYM_HDR
echo 'static size_t kernel_symbol_count = sizeof(kernel_symbols) / sizeof(kernel_symbols[0]);' >> $SYM_HDR
echo "Generated $SYM_HDR"