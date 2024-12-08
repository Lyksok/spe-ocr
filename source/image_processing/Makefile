# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -O2 -I. -fsanitize=address -g
PKGS = `pkg-config --cflags --libs gtk+-3.0`
LIBS = -lSDL2 -lm -lSDL2_image

# Directories
SRC_DIR = .
BIN_DIR = bin
OBJ_DIR = obj

# Source files and object files
SRC_FILES := $(wildcard $(SRC_DIR)/**/*.c $(SRC_DIR)/*.c) $(wildcard ../detection-and-segmentation/**/*.c ../detection-and-segmentation/*.c)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

# Target executable
TARGET = $(BIN_DIR)/main

# Rules
.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS) $(PKGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBS) $(PKGS)

clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)

