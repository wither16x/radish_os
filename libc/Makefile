NASM := nasm
CC := x86_64-radishos-clang

CFLAGS := -ffreestanding \
	-fno-stack-protector \
	-fno-stack-check \
	-fno-lto \
	-fno-pic \
	-fno-pie \
	-mno-red-zone \
	-Wall \
	-Wextra \
	-nostdlib \
	-nostdinc \
	-Iinclude \
	-std=c23 \
	-nostdlib

SRC := .
BUILD := build

ASM_SRCS := $(shell find $(SRC) -type f -name "*.asm")
C_SRCS := $(shell find $(SRC) -type f -name "*.c")
ASM_OBJS := $(patsubst ./%, $(BUILD)/%, $(ASM_SRCS:.asm=.o))
C_OBJS := $(patsubst ./%, $(BUILD)/%, $(C_SRCS:.c=.o))
OBJS := $(ASM_OBJS) $(C_OBJS)
TARGET := $(BUILD)/libc.a

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	ar rcs $(TARGET) $(OBJS)

$(BUILD)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: %.asm
	@mkdir -p $(dir $@)
	$(NASM) -f elf64 $< -o $@

clean:
	rm -rf $(BUILD)