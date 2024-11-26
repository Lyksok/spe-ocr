TARGET = neural
TEST = convertion

CC = gcc

FLAGS = -Wall -Wextra
CFLAGS = -Wall -Wextra -Werror -fsanitize=address -g
LIBS = -lSDL2 -lSDL2_image -lm

OBJ = neural_letter.o
OBJECT = convertion.o main.o

all: $(TARGET) $(TEST)

$(TARGET): $(OBJ)
	$(CC) -o $@ $(OBJ) $(CFLAGS) $(LIBS)

$(TEST): $(OBJECT)
	$(CC) -o $@ $(OBJECT) $(CFLAGS) $(LIBS)

%.o: %.c
	$(CC) -c $< $(CFLAGS) -o $@

clean:
	rm -f $(TARGET) $(TEST) $(OBJ) $(OBJECT)

run: $(TARGET)
	./$(TARGET)

t: $(TEST)
	./$(TEST)

.PHONY: all clean run t
