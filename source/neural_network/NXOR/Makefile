EXEC = nxor

CC = gcc

DEBUG = -fsanitize=address -g
CFLAGS = -Wall -Wextra $(DEBUG)

SRC = neural_nxor.c

all: $(EXEC)

$(EXEC): $(SRC)
	$(CC) $(CFLAGS) -o $(EXEC) $(SRC) -lm

clean:
	rm -f $(EXEC)

run: $(EXEC)
	./$(EXEC)

.PHONY: all clean run
