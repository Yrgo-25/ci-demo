# Application target.
TARGET := firmware

# C++ compiler.
CXX_COMPILER := g++

# C++ compiler flags.
CXX_FLAGS := -Wall -Werror -std=c++17 -Iinclude

# Source files.
SRC_FILES := source/main.cpp \

# Build and run the target by default.
default: build run

# Build the target.
build:
	@$(CXX_COMPILER) $(SRC_FILES) -o $(TARGET) $(CXX_FLAGS)

# Run the target.
run:
	@./$(TARGET)

# Clean the target.
clean:
	@rm -f $(TARGET)

# Analyze format on all .cpp and .h files in the repo.
check-format:
	@clang-format --dry-run --Werror $$(find . -name '*.cpp') $$(find . -name '*.h')

# Format all .cpp and .h files in the repo.
format:
	@clang-format -i $$(find . -name '*.cpp') $$(find . -name '*.h')
