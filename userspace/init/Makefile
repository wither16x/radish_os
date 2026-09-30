TOOL_PREFIX := \$(HOME)/.local/radish-toolchain/bin/
LIBC_PREFIX := \$(HOME)/.local

BUILD := ./build
BIN := ./bin

SRC := ./init.c
OBJ := $(BUILD)/init.o
EXEC = $(BIN)/init

CC := $(TOOL_PREFIX)clang
LD := $(TOOL_PREFIX)clang

CFLAGS := --target=x86_64-unknown-radish -fno-lto \
	-fno-pic \
	-fno-pie \
	-Wall \
	-Wextra \
	-std=c99

LDFLAGS := -static \
	-nostdlib \
	-Wl,-Ttext=0x400000 \
	-z max-page-size=0x1000 \
	-fuse-ld=lld \
	-L$(LIBC_PREFIX)

.PHONY: all clean

all: $(EXEC)

$(EXEC): $(OBJ)
	@mkdir -p $(BIN)
	$(LD) $(LDFLAGS) $(OBJ) -lc -o $(EXEC)

$(OBJ): $(SRC)
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $(SRC) -o $(OBJ)

clean:
	rm -rf $(BUILD) $(BIN)