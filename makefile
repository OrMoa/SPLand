# Define the name of the output binary
TARGET = bin/run

# Define the source and include directories
SRC_DIR = src
INC_DIR = include

# Find all source files in the source directory
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
# Define the object files to be created directly in the bin directory
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=bin/%.o)

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -I$(INC_DIR) -Wall -Weffc++ -g

# Default target: build the executable
all: $(TARGET)

# Build the executable by linking all object files
$(TARGET): $(OBJECTS) | bin
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

# Rule to build object files from source files
bin/%.o: $(SRC_DIR)/%.cpp | bin
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create the bin directory if it doesn't exist
bin:
	mkdir -p bin

# Clean up all generated files
clean:
	rm -rf bin/*