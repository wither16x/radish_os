include ../config.mk

C_FLAGS += -I../$(INCLUDE_DIR)
CXX_FLAGS += -I../$(INCLUDE_DIR)

SOURCE_DIR              = .
OBJECTS_DIR             = ../build/proc
ASM_DIR                 = $(SOURCE_DIR)/asm

CXX_FILES               = $(SOURCE_DIR)/elf.cpp \
                        $(SOURCE_DIR)/exec.cpp \
                        $(SOURCE_DIR)/fork.cpp \
                        $(SOURCE_DIR)/kill.cpp \
                        $(SOURCE_DIR)/pid.cpp \
                        $(SOURCE_DIR)/process.cpp \
                        $(SOURCE_DIR)/procheap.cpp \
			$(SOURCE_DIR)/procstack.cpp \
			$(SOURCE_DIR)/scheduler.cpp \
			$(SOURCE_DIR)/spawn.cpp \
			$(SOURCE_DIR)/wait.cpp

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