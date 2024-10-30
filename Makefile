CC = gcc
FLAGS = -Wall -Wextra -fsanitize=address


# Imported packages
PKGS = `pkg-config --cflags --libs gtk+-3.0`
LIBS = -lSDL2 -lSDL2_image

# Source files
SRC = source/gtk/gtk_app.c source/gtk/splash_screen.c

# Output executable
TARGET = app_launcher


all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(PKGS) $(LIBS) $(FLAGS)

clean:
	rm -f $(TARGET)

# Phony targets
.PHONY: all clean