include ../config.mk

C_FLAGS += -I../$(INCLUDE_DIR)
CXX_FLAGS += -I../$(INCLUDE_DIR)

SOURCE_DIR              = .
OBJECTS_DIR             = ../build/cpu
ASM_DIR                 = $(SOURCE_DIR)/asm

CXX_FILES               = $(SOURCE_DIR)/exception.cpp \
                        $(SOURCE_DIR)/gdt.cpp \
                        $(SOURCE_DIR)/idt.cpp \
                        $(SOURCE_DIR)/irq.cpp \
                        $(SOURCE_DIR)/sse2.cpp \
                        $(SOURCE_DIR)/syscall.cpp \
                        $(SOURCE_DIR)/tss.cpp

CXX_OBJECTS             = $(patsubst ./%, $(OBJECTS_DIR)/%, $(CXX_FILES:.cpp=.o))

.PHONY: all clean

all: asm

asm: $(CXX_OBJECTS)
	$(MAKE) -C $(ASM_DIR)

$(OBJECTS_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX_COMPILER) $(CXX_FLAGS) -c $< -o $@

clean:
	$(MAKE) -C $(ASM_DIR) clean
	rm -rf $(OBJECTS_DIR)