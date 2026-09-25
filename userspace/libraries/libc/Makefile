# change both macros below if needed
PREFIX := x86_64-radishos
LIBC_PREFIX := \$(HOME)/.local

NASM := nasm
CC := $(PREFIX)-clang

CFLAGS := -ffreestanding \
	-fno-stack-protector \
	-fno-stack-check \
	-fno-lto \
	-fno-pic \
	-fno-pie \
	-mno-red-zone \
	-Wall \
	-Wextra \
	-Iinclude \
	-std=c99 \
	-nostdlib

SRC := src
BUILD := build

ASM_SRCS := $(shell find $(SRC) -type f -name "*.asm")
C_SRCS := $(shell find $(SRC) -type f -name "*.c")
ASM_OBJS := $(patsubst $(SRC)/%, $(BUILD)/%, $(ASM_SRCS:.asm=.o))
C_OBJS := $(patsubst $(SRC)/%, $(BUILD)/%, $(C_SRCS:.c=.o))
OBJS := $(ASM_OBJS) $(C_OBJS)
TARGET := $(BUILD)/libc.a

.PHONY: all clean re install build-install

all: $(TARGET)

$(TARGET): $(OBJS)
	ar rcs $(TARGET) $(OBJS)

$(BUILD)/%.o: $(SRC)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: $(SRC)/%.asm
	@mkdir -p $(dir $@)
	$(NASM) -f elf64 $< -o $@

clean:
	rm -rf $(BUILD)

re: clean all

install:
	./mksysroot.sh
	./install.sh $(LIBC_PREFIX)

build-install: re install