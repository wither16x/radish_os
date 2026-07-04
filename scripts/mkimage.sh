#!/bin/bash

set -e

KERNEL="kernel/bin/kernel.elf"

mkdir -p images

if [ "$1" == "--iso" ]; then
      ISO_DIR="iso_root"
      LIMINE_CONF="limine.conf"
      ISO="images/radish_os.iso"

      mkdir -p "$ISO_DIR"
      mkdir -p "$ISO_DIR/boot"
      mkdir -p "$ISO_DIR/boot/limine"
      mkdir -p "$ISO_DIR/EFI/BOOT"

      cp -v "$KERNEL" "$ISO_DIR/boot"
      cp -v "$LIMINE_CONF" \
            .dependencies/Limine/limine-bios.sys \
            .dependencies/Limine/limine-bios-cd.bin \
            .dependencies/Limine/limine-uefi-cd.bin \
            "$ISO_DIR/boot/limine"
      cp -v .dependencies/Limine/BOOTX64.EFI "$ISO_DIR/EFI/BOOT"
      cp -v .dependencies/Limine/BOOTIA32.EFI "$ISO_DIR/EFI/BOOT"
      cp -v .build_initrd/initrd.tar "$ISO_DIR/initrd.tar"

      xorriso -as mkisofs \
            -R \
            -r \
            -J \
            -b boot/limine/limine-bios-cd.bin \
            -no-emul-boot \
            -boot-load-size 4 \
            -boot-info-table \
            -hfsplus \
            -apm-block-size 2048 \
            --efi-boot boot/limine/limine-uefi-cd.bin \
            -efi-boot-part \
            --efi-boot-image \
            --protective-msdos-label \
            "$ISO_DIR" \
            -o "$ISO"

      .dependencies/Limine/limine bios-install "$ISO"
elif [ "$1" == "--floppy" ]; then
      echo "WARNING: image type \"floppy\" is only supported by the Lichee bootloader \
which is unstable and cannot even load the kernel for now"

      dd if=/dev/zero of=images/radish_os.img bs=512 count=2880
      dd if=bootloader/bootsector/bin/floppy.bin of=images/radish_os.img bs=512 seek=0 conv=notrunc
      dd if=bootloader/stage2/bin/stage2.bin of=images/radish_os.img bs=512 seek=1 conv=notrunc
else
      printf "Unsupported image type: %s\n" "$1"
      exit 1
fi