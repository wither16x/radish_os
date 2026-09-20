include ../config.mk

C_FLAGS += -I../$(INCLUDE_DIR)
CXX_FLAGS += -I../$(INCLUDE_DIR)

SOURCE_DIR              = .
OBJECTS_DIR             = ../build/boot
BOOTLOADERS_DIR		= $(SOURCE_DIR)/bootloaders

.PHONY: all clean

all: bootloaders

bootloaders: current
	$(MAKE) -C $(BOOTLOADERS_DIR)

current:
	@mkdir -p $(OBJECTS_DIR)

clean:
	$(MAKE) -C $(BOOTLOADERS_DIR) clean
	rm -rf $(OBJECTS_DIR)