CC = gcc
CPFLAGS =
CFLAGS = -Wall -Wextra
LDFLAGS =
LDLIBS = -lm -lSDL2 -lSDL2_image

BIN = main.out

SRC := $(shell find ./ -name '*.c')
OBJS = ${SRC:.c=.o}

all: $(BIN)

$(BIN): $(OBJS)

debug: CFLAGS += -fsanitize=address -g
debug: LDFLAGS += -fsanitize=address
debug: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(LDLIBS) $(OBJS) -o $(BIN)

.PHONY: clean

clean:
	$(RM) $(OBJS)
	$(RM) $(BIN)
