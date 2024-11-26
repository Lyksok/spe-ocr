CC = gcc
FLAGS = -Wall -Wextra -Werror
OPT = -g -fsanitize=address
LIBS = -lm -lSDL2 -lSDL2_image

SOURCEDIR = ./
SRC := $(shell find $(SOURCEDIR) -name '*.c')
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}
TARGET = main.out

$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(SRC) $(FLAGS) $(OPT) $(LIBS)

.PHONY: clean

clean:
	$(RM) ${OBJ}
	$(RM) $(TARGET)
