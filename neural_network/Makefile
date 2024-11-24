TARGET = neural

CC = gcc

FLAGS = -Wall -Wextra
CFLAGS = -Wall -Wextra -Werror -fsanitize=address -g
LIBS = -lSDL2 -lSDL2_image -lm

SRC = neural_letter.c convertion.c
OBJ = neural_letter.o convertion.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $(OBJ) $(CFLAGS) $(LIBS)

%.o: %.c
	$(CC) -c $< $(CFLAGS) -o $@

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
