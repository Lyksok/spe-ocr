CC = gcc
CFLAGS = `pkg-config --cflags gtk+-3.0`
LDFLAGS = `pkg-config --libs gtk+-3.0`


# Imported packages
PKGS = `pkg-config --cflags --libs gtk+-3.0`
LIBS = -lSDL2 -lSDL2_image

# Source files
SOURCES = source/gtk/gtk_app.c source/gtk/splash_screen.c

# Output executable
TARGET = app_launcher


all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES) $(LDFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
