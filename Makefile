CC = gcc
FLAGS = -Wall -Wextra -fsanitize=address

# Imported packages
PKGS = `pkg-config --cflags --libs gtk+-3.0`
LIBS = -lSDL2 -lSDL2_image -lm

# Source files
SRC_BINARIZATION = source/binarization/*.c
SRC_GTK = source/gtk/*.c 

# Output executables
TARGET_BIN_LOCAL = source/binarization/global_binarization
TARGET_BIN_GLOBAL = source/binarization/local_binarization
TARGET_APP = app_launcher


TARGETS = $(TARGET_BIN_GLOBAL) $(TARGET_BIN_LOCAL) $(TARGET_APP)

$(TARGET_BIN_GLOBAL): $(SRC_BINARIZATION)
	$(CC) $(SRC_BINARIZATION) -o $(TARGET_BIN_GLOBAL) $(PKGS) $(LIBS) $(FLAGS)

$(TARGET_BIN_LOCAL): $(SRC_BINARIZATION)
	$(CC) $(SRC_BINARIZATION) -o $(TARGET_BIN_LOCAL) $(PKGS) $(LIBS) $(FLAGS)

$(TARGET_APP): $(SRC_GTK) $(SRC_BINARIZATION)
	$(CC) $(SRC_GTK) $(SRC_BINARIZATION) -o $(TARGET_APP) $(PKGS) $(LIBS) $(FLAGS)

all: $(TARGETS)
binarization: $(TARGET_BIN_GLOBAL) $(TARGET_BIN_LOCAL)
app: $(TARGET_APP)

clean all:
	rm -f $(TARGET_APP) $(TARGET_BIN_GLOBAL) $(TARGET_BIN_LOCAL)
clean app:
	rm -f $(TARGET_APP)
clean binarization:
	rm -f $(TARGET_BIN_GLOBAL) $(TARGET_BIN_LOCAL)

# Phony targets
.PHONY: all clean