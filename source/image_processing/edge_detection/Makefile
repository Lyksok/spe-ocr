CC = gcc
CFLAGS = -Wall -Wextra -fsanitize=address
LIBS = -lm -lSDL2 -lSDL2_image
PKGS = `pkg-config --cflags --libs gtk+-3.0`

TARGET = canny
SRC = canny.c convolution.c ../utils/converting.c ../utils/pixel_utils.c  ../utils/histograms.c gaussian.c
OBJS = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LIBS) $(PKGS)

%.o: %.c
	$(CC) $(CFLAGS) $(PKGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: all clean