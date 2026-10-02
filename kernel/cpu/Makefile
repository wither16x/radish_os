include ../config.mk
include ../lib.mk

C_FLAGS += -I../$(INCLUDE_DIR)
CXX_FLAGS += -I../$(INCLUDE_DIR)

SOURCE_DIR              = .
OBJECTS_DIR             = ../build/cpu
ASM_DIR                 = $(SOURCE_DIR)/asm
SYSCALLS_DIR		= $(SOURCE_DIR)/syscalls

CXX_FILES               = $(SOURCE_DIR)/exception.cpp \
	                $(SOURCE_DIR)/gdt.cpp \
	                $(SOURCE_DIR)/idt.cpp \
	                $(SOURCE_DIR)/irq.cpp \
	                $(SOURCE_DIR)/sse2.cpp \
	                $(SOURCE_DIR)/tss.cpp

CXX_OBJECTS             = $(patsubst ./%, $(OBJECTS_DIR)/%, $(CXX_FILES:.cpp=.o))

.PHONY: all clean

all: subdirs

subdirs: $(CXX_OBJECTS)
	@$(MAKE) -C $(SYSCALLS_DIR)
	@$(MAKE) -C $(ASM_DIR)

$(OBJECTS_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(call log_info,Compiling cpu/$< to $@...)
	@$(CXX_COMPILER) $(CXX_FLAGS) -c $< -o $@
	$(OK)

clean:
	@$(MAKE) -C $(SYSCALLS_DIR) clean
	@$(MAKE) -C $(ASM_DIR) clean
	@rm -rf $(OBJECTS_DIR)