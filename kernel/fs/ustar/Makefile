include ../../config.mk

C_FLAGS += -I../../$(INCLUDE_DIR)
CXX_FLAGS += -I../../$(INCLUDE_DIR)

SOURCE_DIR              = .
OBJECTS_DIR             = ../../build/fs/ustar

CXX_FILES               = $(SOURCE_DIR)/archive.cpp \
			$(SOURCE_DIR)/dir.cpp \
			$(SOURCE_DIR)/filesystem.cpp \
			$(SOURCE_DIR)/file.cpp \
			$(SOURCE_DIR)/node.cpp

CXX_OBJECTS             = $(patsubst ./%, $(OBJECTS_DIR)/%, $(CXX_FILES:.cpp=.o))

.PHONY: all clean

all: $(CXX_OBJECTS)

$(OBJECTS_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX_COMPILER) $(CXX_FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJECTS_DIR)