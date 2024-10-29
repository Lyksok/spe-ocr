# Compiler
CC = gcc
FLAGS = -Wall -Wextra -Werror -fsanitize=address

# Imported packages
PKGS = `pkg-config --cflags --libs gtk+-3.0`
LIBS = -lSDL2 -lSDL2_image

# Source files
SRC = main.c global_thresholding.c converting.c histograms.c
SRC2 = local_thresholding.c

# Output executable
TARGET = GlobalBinarization
TARGET2 = LocalBinarization

# Build target
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(PKGS) $(LIBS) $(FLAGS)

$(TARGET2): $(SRC2)
	$(CC) $(SRC2) -o $(TARGET2) $(PKGS) $(LIBS) $(FLAGS)

# Clean target
clean:
	rm -f $(TARGET) $(TARGET2)

# Phony targets
.PHONY: all clean
