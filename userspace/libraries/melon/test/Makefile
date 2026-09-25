BUILD := build

CXX := x86_64-radishos-clang++
CXXFLAGS := -fno-lto -fno-pic -fno-pie -std=c++23 -Wall -Wextra -I../include -I. -g

SRCS := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp,$(BUILD)/%.o,$(SRCS))

TARGET := $(BUILD)/test

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS) ../build/libmelon.a
	$(CXX) -fno-pie -no-pie $^ -o $@

$(BUILD)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD)