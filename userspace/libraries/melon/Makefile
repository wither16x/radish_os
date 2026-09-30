INSTALL_DIR := $(HOME)/.local
PREFIX := \$(HOME)/.local/radish-toolchain/bin/
LIBC_PREFIX := \$(HOME)/.local

NASM := nasm
CXX := $(PREFIX)clang++

CFLAGS := --target=x86_64-unknown-radish -fno-lto \
	-fno-pic \
	-fno-pie \
	-Wall \
	-Wextra \
	-Iinclude \
	-isystem $(LIBC_PREFIX) \
	-std=c++23 \
	-nostdlib

SRC := src
BUILD := build

ASM_SRCS := $(shell find $(SRC) -type f -name "*.asm")
CXX_SRCS := $(shell find $(SRC) -type f -name "*.cpp")

ASM_OBJS := $(patsubst $(SRC)/%,$(BUILD)/%,$(ASM_SRCS:.asm=.o))
CXX_OBJS := $(patsubst $(SRC)/%,$(BUILD)/%,$(CXX_SRCS:.cpp=.o))

OBJS := $(ASM_OBJS) $(CXX_OBJS)
TARGET := $(BUILD)/libmelon.a

.PHONY: all clean re install-local install-global

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	ar rcs $(TARGET) $(OBJS)

$(BUILD)/%.o: $(SRC)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: $(SRC)/%.asm
	@mkdir -p $(dir $@)
	$(NASM) -f elf64 $< -o $@

clean:
	rm -rf $(BUILD)

re: clean all

install-local:
	./install.sh $(INSTALL_DIR)

install-global:
	./install.sh