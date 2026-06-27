# tools
CXX      := clang++
CC       := clang
LD       := clang++
ASM      := nasm

# dirs
SRC_DIR  := .
BUILD    := build
BIN      := bin

TARGET   := $(BIN)/kernel.elf
LDSCRIPT := kernel.lds

# sources
CPP_SRCS := $(shell find $(SRC_DIR) -type f -name "*.cpp")
C_SRCS   := $(shell find $(SRC_DIR) -type f -name "*.c")
ASM_SRCS := $(shell find $(SRC_DIR) -type f -name "*.asm")

# objects
CPP_OBJS := $(patsubst ./%, $(BUILD)/%, $(CPP_SRCS:.cpp=.o))
C_OBJS   := $(patsubst ./%, $(BUILD)/%, $(C_SRCS:.c=.o))
ASM_OBJS := $(patsubst ./%, $(BUILD)/%, $(ASM_SRCS:.asm=.o))

OBJS := $(CPP_OBJS) $(C_OBJS) $(ASM_OBJS)

# flags
COMMON_FLAGS := \
	-ffreestanding \
	-fno-stack-protector \
	-fno-stack-check \
	-fno-lto \
	-fno-pic \
	-fno-pie \
	-m64 \
	-mno-red-zone \
	-Wall \
	-Wextra \
	-nostdlib \
	-nostdinc \
	-mcmodel=kernel \
	--target=x86_64-unknown-none \
	-I.

CXXFLAGS := $(COMMON_FLAGS) \
	-std=c++23 \
	-fno-exceptions \
	-fno-rtti \
	-fno-threadsafe-statics \
	-fno-use-cxa-atexit \
	-fno-unwind-tables \
	-fno-asynchronous-unwind-tables \
	-fno-builtin \
	-fvisibility=hidden \
	-fno-sized-deallocation
	
CFLAGS := $(COMMON_FLAGS) \
	-std=c23 \
	-fno-builtin \
	-fvisibility=hidden

ASMFLAGS := -f elf64

LDFLAGS := \
	-fuse-ld=lld \
	-static \
	-nostdlib \
	-Wl,--build-id=none \
	-Wl,-z,max-page-size=0x1000 \
	-Wl,-T,$(LDSCRIPT)

# build rules
all: $(TARGET)

# link
$(TARGET): $(OBJS)
	@mkdir -p $(BIN)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

# c++
$(BUILD)/%.o: ./%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# c
$(BUILD)/%.o: ./%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# asm
$(BUILD)/%.o: ./%.asm
	@mkdir -p $(dir $@)
	$(ASM) $(ASMFLAGS) $< -o $@

# utils
clean:
	rm -rf $(BUILD) $(BIN)

re: clean all

.PHONY: all clean re