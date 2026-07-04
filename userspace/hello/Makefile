BUILD := ./build
BIN := ./bin

SRC := ./hello.c
OBJ := $(BUILD)/hello.o
EXEC = $(BIN)/hello

CC := $(PREFIX)-clang
LD := $(PREFIX)-clang

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
	-mcmodel=small \
	-std=c23 \
	-fno-builtin \
	-fvisibility=hidden

LDFLAGS := -static \
	-nostdlib \
	-Wl,-Ttext=0x400000 \
	-Wl,--build-id=none \
	-z max-page-size=0x1000 \
	-fuse-ld=lld

.PHONY: all clean

all: $(EXEC)

$(EXEC): $(OBJ)
	@mkdir -p $(BIN)
	$(LD) $(LDFLAGS) $(OBJ) -o $(EXEC)

$(OBJ): $(SRC)
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $(SRC) -o $(OBJ)

clean:
	rm -rf $(BUILD) $(BIN)