CC = gcc

TARGET1 = test
TARGET2 = solver
SOURCES1 = solver.c test.c
SOURCES2 = solver.c read.c main.c
FLAGS = -Wall -Wextra -Werror

# all: $(TARGET1) $(TARGET2)
all: $(TARGET2)

$(TARGET1): $(SOURCES1)
	$(CC) -o $(TARGET1) $(SOURCES1) $(FLAGS)

$(TARGET2): 
	$(CC) -o $(TARGET2) $(SOURCES2) $(FLAGS)

clean:
	rm -f $(TARGET1) $(TARGET2)
