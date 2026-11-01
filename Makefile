# This file contains a make script for the CabinetConverter application
#
# Author: Josh McIntyre
#

# This block defines makefile variables
CORE_FILES=src/*.cpp
CORE_TESTONLY_FILES := $(filter-out src/cabinetconverter.cpp, $(wildcard src/*.cpp))
TEST_FILES=tests/*.cpp
INCLUDE_CORE=src/
INCLUDE_TESTS=tests/

BUILD_DIR=bin
BUILD_BIN=cabinetconverter
BUILD_TEST_BIN=test_cabinetconverter

CC=g++
FLAGS= -std=c++17 -Werror -I$(INCLUDE_CORE) -I$(INCLUDE_TESTS)

# This rule builds the utility
build: $(CORE_FILES)
	mkdir -p $(BUILD_DIR)
	$(CC) $(FLAGS) -o $(BUILD_DIR)/$(BUILD_BIN) $(CORE_FILES)

# This rule builds the unit tests
tests: $(TEST_FILES) $(CORE_TESTONLY_FILES)
	mkdir -p $(BUILD_DIR)
	$(CC) $(FLAGS) -o $(BUILD_DIR)/$(BUILD_TEST_BIN) $(TEST_FILES) $(CORE_TESTONLY_FILES)

# This rule cleans the build directory
clean: $(BUILD_DIR)
	rm $(BUILD_DIR)/* 
	rmdir $(BUILD_DIR)
