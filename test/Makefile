# Test suite target.
TARGET := testsuite

# Test files.
TEST_FILES := driver/timer/test_generic.cpp \
              driver/timer/test_stub.cpp \
              main.cpp \

# Main include directory, i.e. the interfaces and drivers under test.
INC_DIR := ../include

# Directory containing the yrgo::test framework submodule.
YRGO_TEST_DIR := ../libs/test
YRGO_TEST_LIB := $(YRGO_TEST_DIR)/libyrgo_test.a

# C++ compiler.
CXX_COMPILER := g++

# C++ compiler flags.
CXX_FLAGS := -Wall -Werror -std=c++17 -I. -I$(INC_DIR) -I$(YRGO_TEST_DIR)/include

# Build and run the test suite by default.
default: build run

# Build the yrgo::test library.
$(YRGO_TEST_LIB):
	@$(MAKE) -C $(YRGO_TEST_DIR) lib

# Build the test suite.
build: $(YRGO_TEST_LIB)
	@$(CXX_COMPILER) $(TEST_FILES) -o $(TARGET) $(CXX_FLAGS) -L$(YRGO_TEST_DIR) -lyrgo_test

# Run the test suite.
run: build
	@./$(TARGET)

# Clean the test suite and the yrgo::test library.
clean:
	@rm -f $(TARGET)
	@$(MAKE) -C $(YRGO_TEST_DIR) clean

.PHONY: default build run clean
