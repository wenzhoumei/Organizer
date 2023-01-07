# Specify the target executable
TARGET = files/program.program

# Specify the C++ compiler
CXX = g++

# Specify the C++ compiler flags
CFLAGS = -g -Wall -Wextra -std=c++20

# Specify the C++ linker flags
LDFLAGS =

# Specify the source files
SRCS = $(wildcard *.cpp)

# Specify the object files (automatically generated from the source files)
OBJ = $(SRCS:.cpp=.o)

# Default target
all: $(TARGET)

# Build the target executable
$(TARGET): $(OBJ)
	$(CXX) $(CFLAGS) $(OBJ) $(LDFLAGS) -o $(TARGET)

# Compile the source files into object files
%.o: %.cpp
	$(CXX) $(CFLAGS) -c $<

# Cleanup
clean:
	rm -f $(TARGET) $(OBJ)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
