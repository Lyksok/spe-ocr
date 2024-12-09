TARGET = neural
TEST = convertion

CC = gcc

#FLAGS = -Wall -Wextra
#BFLAGS = -Wall -Wextra -Werror -fsanitize=address -g
DEBUG = -fsanitize=address -g
CFLAGS = -Wall -Wextra
PKGS = `pkg-config --cflags --libs gtk+-3.0`
LIBS = -lSDL2 -lSDL2_image -lm

OBJ2 = convertion.o ../gtk/conversion_utils.o ../gtk/image_utils.o
OBJ = neural.o setup.o csv.o read_data.o $(OBJ2)
#OBJECT = convertion.o main.o ../gtk/conversion_utils.o ../gtk/image_utils.o

#all: $(TARGET) $(TEST)
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $(OBJ) $(CFLAGS) $(LIBS) $(PKGS)

#$(TEST): $(OBJECT)
#	$(CC) -o $@ $(OBJECT) $(CFLAGS) $(LIBS) $(PKGS)

%.o: %.c
	$(CC) -c $< $(CFLAGS) -o $@ $(LIBS) $(PKGS)

clean:
	rm -f $(TARGET) $(OBJ)
#	rm -f $(TARGET) $(TEST) $(OBJ) $(OBJECT)

run: $(TARGET)
	./$(TARGET)

#t: $(TEST)
#	./$(TEST)

#.PHONY: all clean run t
.PHONY: all clean run
