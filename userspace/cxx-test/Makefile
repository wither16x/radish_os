include ../../prefixes.mk

BUILD := ./build
BIN := ./bin

SRC := ./cxx_test.cpp
OBJ := $(BUILD)/cxx_test.o
EXEC = $(BIN)/cxx_test

CC := $(TOOL_PREFIX)-clang++
LD := $(TOOL_PREFIX)-clang++

CFLAGS := -fno-lto \
	-fno-pic \
	-fno-pie \
	-Wall \
	-Wextra \
	-std=c++23

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