include ../config.mk

C_FLAGS += -I../$(INCLUDE_DIR)
CXX_FLAGS += -I../$(INCLUDE_DIR)

SOURCE_DIR              = .
OBJECTS_DIR             = ../build/boot
BOOTLOADERS_DIR		= $(SOURCE_DIR)/bootloaders

CXX_FILES               = $(SOURCE_DIR)/bootinfo.cpp

CXX_OBJECTS             = $(patsubst ./%, $(OBJECTS_DIR)/%, $(CXX_FILES:.cpp=.o))

.PHONY: all clean

all: bootloaders

bootloaders: $(CXX_OBJECTS)
	$(MAKE) -C $(BOOTLOADERS_DIR)

$(OBJECTS_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX_COMPILER) $(CXX_FLAGS) -c $< -o $@

clean:
	$(MAKE) -C $(BOOTLOADERS_DIR) clean
	rm -rf $(OBJECTS_DIR)