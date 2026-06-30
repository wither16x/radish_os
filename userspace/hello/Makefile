BUILD := ./build
BIN := ./bin

SRC := ./hello.c
OBJ := $(BUILD)/hello.o
EXEC = $(BIN)/hello

CC := x86_64-elf-gcc
LD := ld

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
	-mcmodel=small \
	-std=c23 \
	-fno-builtin \
	-fvisibility=hidden

LDFLAGS := -static \
	-nostdlib \
	-Ttext=0x400000 \
	--build-id=none \
	-z max-page-size=0x1000

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