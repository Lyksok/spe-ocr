CC = gcc
FLAGS = -Wall -Wextra -Werror
OPT = -g -fsanitize=address
LIBS = -lm -lSDL2 -lSDL2_image

SRC = main.c binarizing.c helper.c sauvola.c converting.c histograms.c *_thresholding.c
TARGET = main.out

$(TARGET): $(SRC)
	$(CC) -o $(TARGET) $(SRC) $(FLAGS) $(OPT) $(LIBS)
