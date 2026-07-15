PHONY: build-all build-kernel build-userspace build-initrd build-iso \
	clean-kernel clean-userspace clean-all \
	run debug \
	test

# build
build-all: build-kernel build-userspace build-initrd build-iso

build-kernel:
	./scripts/symbols.sh
	$(MAKE) -C kernel clean
	$(MAKE) -C kernel
	./scripts/symbols.sh

build-userspace:
	$(MAKE) -C userspace/hello
	$(MAKE) -C userspace/shell

build-initrd:
	./scripts/mkinitrd.sh

build-iso:
	./scripts/mkimage.sh --iso

# clean
clean-all: clean-userspace

clean-kernel:
	$(MAKE) -C kernel clean

clean-userspace:
	$(MAKE) -C userspace/hello clean
	$(MAKE) -C userspace/shell clean

# run
run:
	qemu-system-x86_64 -cdrom images/radish_os.iso -m 2G

debug:
	qemu-system-x86_64 -cdrom images/radish_os.iso -m 2G -d int -no-reboot #-s -S

# test
test: clean-all build-all run