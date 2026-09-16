include config.mk

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
	$(LINKER) $(LINKER_FLAGS) -o $@ $(OBJECTS)

nolink:
	$(MAKE) -C $(BOOT_SUBDIR)
	$(MAKE) -C $(CPU_SUBDIR)
	$(MAKE) -C $(DRIVERS_SUBDIR)
	$(MAKE) -C $(FS_SUBDIR)
	$(MAKE) -C $(LIB_SUBDIR)
	$(MAKE) -C $(MEM_SUBDIR)
	$(MAKE) -C $(PROC_SUBDIR)
	$(MAKE) -C $(BASE_SUBDIR)
ifeq ($(BUILD_MODE),test)
	$(MAKE) -C $(TEST_DIR)
endif

clean:
	$(MAKE) -C $(BOOT_SUBDIR) clean
	$(MAKE) -C $(CPU_SUBDIR) clean
	$(MAKE) -C $(DRIVERS_SUBDIR) clean
	$(MAKE) -C $(FS_SUBDIR) clean
	$(MAKE) -C $(LIB_SUBDIR) clean
	$(MAKE) -C $(MEM_SUBDIR) clean
	$(MAKE) -C $(PROC_SUBDIR) clean
	$(MAKE) -C $(BASE_SUBDIR) clean
ifeq ($(BUILD_MODE),test)
	$(MAKE) -C $(TEST_DIR) clean
endif
	rm -rf $(BINARIES_DIR)