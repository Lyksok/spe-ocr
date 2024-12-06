CC = gcc
FLAGS = -Wall -Wextra -fsanitize=address -g


# Imported packages
PKGS = `pkg-config --cflags --libs gtk+-3.0 gtksourceview-4` -rdynamic
LIBS = -lSDL2 -lSDL2_image -lm

# Source files
IGNORE = %/image_processing.c %/main.c $(wildcard source/image_processing/detection-and-segmentation/*) source/image_processing/main.c
SRC_APP = source/gtk/*.c
SRC_BIN := $(filter-out $(IGNORE), $(wildcard source/image_processing/*/*.c))

# Output executable
TARGET = app_launcher


all: $(TARGET)

$(TARGET): $(SRC_BIN) $(SRC_APP)
	$(CC) $(SRC_APP) $(SRC_BIN) -o $(TARGET) $(PKGS) $(LIBS) $(FLAGS)

clean:
	rm -f $(TARGET)

# Phony targets
.PHONY: all clean
