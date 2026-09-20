include ../../config.mk

C_FLAGS += -I../../$(INCLUDE_DIR)
CXX_FLAGS += -I../../$(INCLUDE_DIR)

SOURCE_DIR              = .
OBJECTS_DIR             = ../../build/boot/bootloaders

CXX_FILES               = $(SOURCE_DIR)/limine.cpp

CXX_OBJECTS             = $(patsubst ./%, $(OBJECTS_DIR)/%, $(CXX_FILES:.cpp=.o))

.PHONY: all clean

all: $(CXX_OBJECTS)

$(OBJECTS_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXX_FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJECTS_DIR)