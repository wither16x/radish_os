include ../config.mk
include ../lib.mk

C_FLAGS += -I../$(INCLUDE_DIR)
CXX_FLAGS += -I../$(INCLUDE_DIR)

SOURCE_DIR              = .
OBJECTS_DIR             = ../build/drivers

CXX_FILES               = $(SOURCE_DIR)/console.cpp \
                        $(SOURCE_DIR)/framebuffer.cpp \
			$(SOURCE_DIR)/keyboard.cpp \
			$(SOURCE_DIR)/pic.cpp \
			$(SOURCE_DIR)/pit.cpp \
			$(SOURCE_DIR)/ps2kbd.cpp \
			$(SOURCE_DIR)/serial.cpp

CXX_OBJECTS             = $(patsubst ./%, $(OBJECTS_DIR)/%, $(CXX_FILES:.cpp=.o))

.PHONY: all clean

all: $(CXX_OBJECTS)

$(OBJECTS_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(call log_info,Compiling drivers/$< to $@...)
	@$(CXX_COMPILER) $(CXX_FLAGS) -c $< -o $@
	$(OK)

clean:
	@rm -rf $(OBJECTS_DIR)