#!/bin/bash

set -e

echo "WARNING: this script aims to configure the Lichee bootloader, which is very unstable and not advanced (it cannot even load the kernel for now). It is strongly recommended to use the Limine (or any bootloader which supports the Limine boot protocol)."

echo "1. Where do you want RadishOS to boot? (supported: floppy)"
read -p "> " boot_device
if [ $boot_device != "floppy" ]; then
        echo "Cannot boot on that."
        exit 1
fi

echo "BOOT_DEVICE := $boot_device" > bootloader/bootsector/bootcfg.mk
echo "Generated bootloader/bootsector/bootcfg.mk"