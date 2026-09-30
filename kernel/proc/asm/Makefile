include ../../config.mk

C_FLAGS += -I../../$(INCLUDE_DIR)
CXX_FLAGS += -I../../$(INCLUDE_DIR)

SOURCE_DIR              = .
OBJECTS_DIR             = ../../build/proc/asm

ASM_FILES               = $(SOURCE_DIR)/proc_switch.asm \
                        $(SOURCE_DIR)/proc_trampoline.asm

ASM_OBJECTS             = $(patsubst ./%, $(OBJECTS_DIR)/%, $(ASM_FILES:.asm=.o))

.PHONY: all clean

all: $(ASM_OBJECTS)

$(OBJECTS_DIR)/%.o: %.asm
	@mkdir -p $(dir $@)
	$(call log_info,Assembling proc/asm/$< to $@...)
	@$(NASM) $(NASM_FLAGS) $< -o $@
	$(OK)

clean:
	@rm -rf $(OBJECTS_DIR)