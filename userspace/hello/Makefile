BUILD := ./build
BIN := ./bin

SRC := ./hello.c
OBJ := $(BUILD)/hello.o
EXEC = $(BIN)/hello

CC := clang
CFLAGS := -ffreestanding \
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
	-std=c23 \
	-fno-builtin \
	--target=x86_64-unknown-none \
	-fvisibility=hidden

LDFLAGS := -fuse-ld=lld \
	-static \
	-nostdlib \
	-Wl,--Ttext=0x400000 \
	-Wl,--build-id=none \
	-Wl,-z,max-page-size=0x1000

.PHONY: all clean

all: $(EXEC)

$(EXEC): $(OBJ)
	@mkdir -p $(BIN)
	$(CC) $(LDFLAGS) $(OBJ) -o $(EXEC)

$(OBJ): $(SRC)
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $(SRC) -o $(OBJ)

clean:
	rm -rf $(BUILD) $(BIN)