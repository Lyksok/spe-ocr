# Compiler
CC = gcc

# Compiler flags
CFLAGS = `pkg-config --cflags gtk4 opencv4` # preprocessor instructions
LDFLAGS = `pkg-config --libs gtk4 opencv4` # linker instructions

# Source files
SRC = source/gtk_app.c

# Output executable
TARGET = source/gtk_app

# Build target
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(LDFLAGS)

# Clean target
clean:
	rm -f $(TARGET)

# Phony targets
.PHONY: all clean