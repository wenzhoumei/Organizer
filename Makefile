# Specify the target executable
TARGET = exec

# Specify the C++ compiler
CC = g++

# Specify the C++ compiler flags
CFLAGS = -Wall -Wextra -std=c++20

# Specify the C++ linker flags
LDFLAGS = sqlite3

# Specify the source files
SRC = main.cpp

# Specify the object files (automatically generated from the source files)
OBJ = $(SRC:.cpp=.o)

# Default target
all: $(TARGET)

# Build the target executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -l $(LDFLAGS) -o $(TARGET)

# Compile the source files into object files
%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

# Cleanup
clean:
	rm -f $(TARGET) $(OBJ)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
