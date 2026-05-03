TARGET_EXEC := loxer

CPP := g++

SRC_DIRS := ./src

BUILD_TYPE ?= release
BUILD_DIR := ./build/$(BUILD_TYPE)

SRCS := $(shell find $(SRC_DIRS) -name '*.cpp')

OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS := $(INC_FLAGS) -MMD -MP 
CXXFLAGS := -std=c++23 -Wall -Wextra

ifeq ($(BUILD_TYPE), debug)
	CXXFLAGS += -g3 -O0 -DDEBUG
else
	CXXFLAGS += -O2 -DNDEBUG
endif

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CPP) $(OBJS) -o $@ 

$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CPP) $(CPPFLAGS) -c $< -o $@

.PHONY: clean release debug

debug:
	$(MAKE) BUILD_TYPE=debug
release:
	$(MAKE) BUILD_TYPE=release
clean:
	rm -rf build/*

-include $(DEPS)
