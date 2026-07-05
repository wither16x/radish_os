CC := clang
LD := clang

SRC := .
BUILD := build
BIN := bin

TARGET := $(BIN)/stage2.bin
LDSCRIPT := stage2.lds
C_SRCS := $(shell find $(SRC) -type f -name "*.c")
C_OBJS := $(patsubst ./%, $(BUILD)/%, $(C_SRCS:.c=.o))

OBJS := $(C_OBJS)

CFLAGS32 := --target="i386-unknown-none" \
	-ffreestanding \
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
	-fno-builtin \
	-fvisibility=hidden

LDFLAGS := --target="i386-unknown-none" \
	-static \
	-nostdlib \
	-Wl,--build-id=none \
	-T $(LDSCRIPT) \
	-fuse-ld=lld

.PHONY: all clean

all: $(TARGET)

# link
$(TARGET): $(OBJS)
	@mkdir -p $(BIN)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

# c
$(BUILD)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS32) -c $< -o $@

clean:
	rm -rf $(BUILD) $(BIN)