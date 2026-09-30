include ../config.mk
include ../lib.mk

C_FLAGS += -I../$(INCLUDE_DIR)
CXX_FLAGS += -I../$(INCLUDE_DIR)

SOURCE_DIR              = .
OBJECTS_DIR             = ../build/mem

CXX_FILES               = $(SOURCE_DIR)/heap.cpp \
                        $(SOURCE_DIR)/page.cpp \
			$(SOURCE_DIR)/pml4t.cpp \
			$(SOURCE_DIR)/pmm.cpp \
			$(SOURCE_DIR)/vmm.cpp

CXX_OBJECTS             = $(patsubst ./%, $(OBJECTS_DIR)/%, $(CXX_FILES:.cpp=.o))

.PHONY: all clean

all: $(CXX_OBJECTS)

$(OBJECTS_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(call log_info,Compiling mem/$< to $@...)
	@$(CXX_COMPILER) $(CXX_FLAGS) -c $< -o $@
	$(OK)

clean:
	@rm -rf $(OBJECTS_DIR)