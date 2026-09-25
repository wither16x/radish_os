BUILD := build

CXX := x86_64-radishos-clang++
CXXFLAGS := -fno-lto -fno-pic -fno-pie -std=c++23 -Wall -Wextra -I.

SRCS := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp,$(BUILD)/%.o,$(SRCS))

TARGET := $(BUILD)/hello_world

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -fno-pie -no-pie $^ -o $@ -lmelon

$(BUILD)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD)