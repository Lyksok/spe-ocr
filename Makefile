CC = gcc
FLAGS = -Wall -Wextra -Werror -fsanitize=address


# Imported packages
PKGS = `pkg-config --cflags --libs gtk+-3.0`
LIBS = -lSDL2 -lSDL2_image -lm

# Source files
SRC_APP = source/gtk/*.c
SRC_BIN = $(filter-out source/binarization/main.c, $(wildcard source/binarization/*.c))

# Output executable
TARGET = app_launcher


all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(SRC_APP) $(SRC_BIN) -o $(TARGET) $(PKGS) $(LIBS) $(FLAGS)

clean:
	rm -f $(TARGET)

# Phony targets
.PHONY: all clean