include bootcfg.mk

NASM := nasm

BIN := bin

.PHONY: all clean

all:
	@mkdir -p $(BIN)
ifeq ($(BOOT_DEVICE),floppy)
	$(NASM) -f bin floppy.asm -o $(BIN)/floppy.bin
else
	@printf "lichee cannot boot on %s\n" "$(BOOT_DEVICE)"
	@exit 1
endif

clean:
	rm -rf $(BIN)