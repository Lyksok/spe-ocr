CC = gcc

TARGET = test
SOURCES = solver.c test.c
FLAGS = -Wall -Wextra -Werror

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) -o $(TARGET) $(SOURCES) $(FLAGS)

clean:
	rm $(TARGET)
