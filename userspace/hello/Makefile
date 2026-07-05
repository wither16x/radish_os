PREFIX := x86_64-radishos

BUILD := ./build
BIN := ./bin

SRC := ./hello.c
OBJ := $(BUILD)/hello.o
EXEC = $(BIN)/hello

CC := $(PREFIX)-clang
LD := $(PREFIX)-clang

CFLAGS := -fno-stack-protector \
	-fno-stack-check \
	-fno-lto \
	-fno-pic \
	-fno-pie \
	-mno-red-zone \
	-Wall \
	-Wextra \
	-std=c23

LDFLAGS := -static \
	-nostdlib \
	-Wl,-Ttext=0x400000 \
	-z max-page-size=0x1000 \
	-fuse-ld=lld \
	-L../../sysroot/usr/lib

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