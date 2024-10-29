EXEC = neural

CC = gcc

CFLAGS = -Wall -Wextra -g

SRC = neural.c

all: $(EXEC)

$(EXEC): $(SRC)
	$(CC) $(CFLAGS) -o $(EXEC) $(SRC) -lm

clean:
	rm -f $(EXEC)

run: $(EXEC)
	./$(EXEC)

.PHONY: all clean run
