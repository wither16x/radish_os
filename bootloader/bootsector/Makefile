# TODO: write a script to generate a bootmacro.mk which contains informations
# for the bootloader
BOOT_ON := floppy

NASM := nasm

BIN := bin

.PHONY: all clean

all:
	@mkdir -p $(BIN)
ifeq ($(BOOT_ON),floppy)
	$(NASM) -f bin floppy.asm -o $(BIN)/floppy.bin
else
	@printf "lichee cannot boot on %s\n" "$(BOOT_ON)"
	@exit 1
endif

clean:
	rm -rf $(BIN)