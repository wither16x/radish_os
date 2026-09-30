include ../../config.mk
include ../../lib.mk

C_FLAGS += -I../../$(INCLUDE_DIR)
CXX_FLAGS += -I../../$(INCLUDE_DIR)

SOURCE_DIR              = .
OBJECTS_DIR             = ../../build/cpu/asm

ASM_FILES               = $(SOURCE_DIR)/enable_sse2.asm \
                        $(SOURCE_DIR)/gdt_flush.asm \
                        $(SOURCE_DIR)/idt_flush.asm \
                        $(SOURCE_DIR)/tss_flush.asm \
                        $(SOURCE_DIR)/irq.asm \
                        $(SOURCE_DIR)/isr.asm \
                        $(SOURCE_DIR)/syscall_common.asm \
                        $(SOURCE_DIR)/userspace.asm

ASM_OBJECTS             = $(patsubst ./%, $(OBJECTS_DIR)/%, $(ASM_FILES:.asm=.o))

.PHONY: all clean

all: $(ASM_OBJECTS)

$(OBJECTS_DIR)/%.o: %.asm
	@mkdir -p $(dir $@)
	$(call log_info,Assembling cpu/asm/$< to $@...)
	@$(NASM) $(NASM_FLAGS) $< -o $@
	$(OK)

clean:
	@rm -rf $(OBJECTS_DIR)