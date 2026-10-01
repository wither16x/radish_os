include config.mk
include lib.mk

MAKEFLAGS += --no-print-directory

# Configuration
SOURCE_DIR			= .
OBJECTS_DIR			= build
BINARIES_DIR			= bin

# Sub directories
BOOT_SUBDIR			= $(SOURCE_DIR)/boot
CPU_SUBDIR			= $(SOURCE_DIR)/cpu
DRIVERS_SUBDIR			= $(SOURCE_DIR)/drivers
FS_SUBDIR			= $(SOURCE_DIR)/fs
LIB_SUBDIR			= $(SOURCE_DIR)/lib
MEM_SUBDIR			= $(SOURCE_DIR)/mem
PROC_SUBDIR			= $(SOURCE_DIR)/proc
BASE_SUBDIR			= $(SOURCE_DIR)/base
PANIC_SUBDIR			= $(SOURCE_DIR)/panic

# Target executable
TARGET				= $(BINARIES_DIR)/kernel

# Object and source files
CXX_SOURCES			= $(shell find $(SOURCE_DIR) -type f -name "*.cpp")
C_SOURCES			= $(shell find $(SOURCE_DIR) -type f -name "*.c")
ASM_SOURCES			= $(shell find $(SOURCE_DIR) -type f -name "*.asm")

OBJECTS				= $(patsubst ./%, $(OBJECTS_DIR)/%, $(CXX_SOURCES:.cpp=.o)) \
				$(patsubst ./%, $(OBJECTS_DIR)/%, $(C_SOURCES:.c=.o)) \
				$(patsubst ./%, $(OBJECTS_DIR)/%, $(ASM_SOURCES:.asm=.o))

# Rules
.PHONY: all nolink clean

all: $(TARGET)

$(TARGET): nolink
	@mkdir -p $(BINARIES_DIR)
	$(call log_info,Linking $(OBJECTS) to $@...)
	@$(LINKER) $(LINKER_FLAGS) -o $@ $(OBJECTS)
	$(OK)

nolink:
	$(call log_note,Build mode: $(BUILD_MODE))
	$(call log_note,Toolchain:)
	$(call log_note,* C compiler: $(C_COMPILER))
	$(call log_note,* C++ compiler: $(CXX_COMPILER))
	$(call log_note,* Linker: $(LINKER))
	$(call log_note,* NASM: $(NASM))

	@$(MAKE) -C $(PANIC_SUBDIR)
	@$(MAKE) -C $(BOOT_SUBDIR)
	@$(MAKE) -C $(CPU_SUBDIR)
	@$(MAKE) -C $(DRIVERS_SUBDIR)
	@$(MAKE) -C $(FS_SUBDIR)
	@$(MAKE) -C $(LIB_SUBDIR)
	@$(MAKE) -C $(MEM_SUBDIR)
	@$(MAKE) -C $(PROC_SUBDIR)
	@$(MAKE) -C $(BASE_SUBDIR)
ifeq ($(BUILD_MODE),test)
	@$(MAKE) -C $(TEST_DIR)
endif

clean:
	$(call log_info,Cleaning build...)

	@$(MAKE) -C $(PANIC_SUBDIR) clean
	@$(MAKE) -C $(BOOT_SUBDIR) clean
	@$(MAKE) -C $(CPU_SUBDIR) clean
	@$(MAKE) -C $(DRIVERS_SUBDIR) clean
	@$(MAKE) -C $(FS_SUBDIR) clean
	@$(MAKE) -C $(LIB_SUBDIR) clean
	@$(MAKE) -C $(MEM_SUBDIR) clean
	@$(MAKE) -C $(PROC_SUBDIR) clean
	@$(MAKE) -C $(BASE_SUBDIR) clean
ifeq ($(BUILD_MODE),test)
	@$(MAKE) -C $(TEST_DIR) clean
endif
	@rm -rf $(BINARIES_DIR)

	$(OK)