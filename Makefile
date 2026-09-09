# Application target.
TARGET := firmware

# Test directory, which holds the test suite and its own makefile.
TEST_DIR := test

# Build and run the target by default.
default: build run

# Build the target.
build:
	@ci/build.sh

# Run the target.
run:
	@./$(TARGET)

# Build and run the unit tests.
unit-test:
	@ci/unit_test.sh

# Analyze format on all C++ files in the repo, without changing anything.
check-format:
	@ci/format.sh --check

# Format all C++ files in the repo.
format:
	@ci/format.sh

# Clean the target, the test suite and the test framework.
clean:
	@rm -f $(TARGET)
	@$(MAKE) -C $(TEST_DIR) clean

.PHONY: default build run unit-test check-format format clean
