include ../config.mk

C_FLAGS += -I../$(INCLUDE_DIR)
CXX_FLAGS += -I../$(INCLUDE_DIR)

SOURCE_DIR              = .
OBJECTS_DIR             = ../build/fs

CXX_FILES               = $(SOURCE_DIR)/default_virtuals.cpp \
                        $(SOURCE_DIR)/devfs.cpp \
			$(SOURCE_DIR)/path.cpp \
			$(SOURCE_DIR)/tmpfs.cpp \
			$(SOURCE_DIR)/ustar.cpp \
			$(SOURCE_DIR)/vfs.cpp

CXX_OBJECTS             = $(patsubst ./%, $(OBJECTS_DIR)/%, $(CXX_FILES:.cpp=.o))

.PHONY: all clean

all: $(CXX_OBJECTS)

$(OBJECTS_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX_COMPILER) $(CXX_FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJECTS_DIR)