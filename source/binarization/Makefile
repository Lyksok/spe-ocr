# Compiler
CC = gcc
FLAGS = -Wall -Wextra -Werror -fsanitize=address

# Imported packages
PKGS = `pkg-config --cflags --libs gtk+-3.0`
LIBS = -lSDL2 -lSDL2_image -lm

# Source files
SRC = main.c binarizing.c histograms.c converting.c global_thresholding.c local_thresholding.c

# Output executable
TARGET = GlobalBinarization
TARGET2 = LocalBinarization

# Build target
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(PKGS) $(LIBS) $(FLAGS)

$(TARGET2): $(SRC)
	$(CC) $(SRC) -o $(TARGET2) $(PKGS) $(LIBS) $(FLAGS)

# Clean target
clean:
	rm -f $(TARGET) $(TARGET2)

# Phony targets
.PHONY: all clean
