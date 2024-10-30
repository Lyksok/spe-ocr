CC = gcc
FLAGS = -Wall -Wextra -fsanitize=address

# Imported packages
PKGS = `pkg-config --cflags --libs gtk+-3.0`
LIBS = -lSDL2 -lSDL2_image -lm

# Source files
SRC_BINARIZATION = $(shell find source/binarization -name '*.c')
SRC_BINARIZATION_NO_MAIN = $(shell find source/binarization -name '*.c' | grep -v 'main.c')
SRC_GTK = source/gtk/*.c

# Output executables
TARGET_BIN_LOCAL = source/binarization/local_binarization
TARGET_BIN_GLOBAL = source/binarization/global_binarization
TARGET_APP = app_launcher

TARGETS = $(TARGET_BIN_GLOBAL) $(TARGET_BIN_LOCAL) $(TARGET_APP)

$(TARGET_BIN_GLOBAL): $(SRC_BINARIZATION)
	$(CC) $(SRC_BINARIZATION) -o $(TARGET_BIN_GLOBAL) $(PKGS) $(LIBS) $(FLAGS)

$(TARGET_BIN_LOCAL): $(SRC_BINARIZATION)
	$(CC) $(SRC_BINARIZATION) -o $(TARGET_BIN_LOCAL) $(PKGS) $(LIBS) $(FLAGS)

$(TARGET_APP): $(SRC_GTK) $(SRC_BINARIZATION_NO_MAIN)
	$(CC) $(SRC_GTK) $(SRC_BINARIZATION_NO_MAIN) -o $(TARGET_APP) $(PKGS) $(LIBS) $(FLAGS)

all: $(TARGET_APP)

binarization: $(TARGET_BIN_GLOBAL) $(TARGET_BIN_LOCAL)

clean:
	rm -f $(TARGET_APP)

clean_bin:
	rm -f $(TARGET_BIN_GLOBAL) $(TARGET_BIN_LOCAL)