include ../config.mk

C_FLAGS += -I../$(INCLUDE_DIR)
CXX_FLAGS += -I../$(INCLUDE_DIR)

SOURCE_DIR              = .
OBJECTS_DIR             = ../build/lib

CXX_FILES               = $(SOURCE_DIR)/alloc.cpp \
                        $(SOURCE_DIR)/conversion.cpp \
			$(SOURCE_DIR)/logging.cpp \
			$(SOURCE_DIR)/memory.cpp \
			$(SOURCE_DIR)/print.cpp \
			$(SOURCE_DIR)/time.cpp

CXX_OBJECTS             = $(patsubst ./%, $(OBJECTS_DIR)/%, $(CXX_FILES:.cpp=.o))

.PHONY: all clean

all: $(CXX_OBJECTS)

$(OBJECTS_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX_COMPILER) $(CXX_FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJECTS_DIR)