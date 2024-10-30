CC = gcc

TARGET1 = test
TARGET2 = solver
SOURCES1 = solver.c test.c
SOURCES2 = solver.c read.c main.c
OBJECTS1 = solver.o test.o
OBJECTS2 = solver.o read.o main.o
FLAGS = -Wall -Wextra -Werror

all: $(TARGET1) $(TARGET2)
#all: $(TARGET2)

#$(TARGET1): $(SOURCES1)
#	$(CC) -o $(TARGET1) $(SOURCES1) $(FLAGS)

#$(TARGET2): 
#	$(CC) -o $(TARGET2) $(SOURCES2) $(FLAGS)

$(TARGET1): $(OBJECTS1)
	$(CC) -o $@ $(OBJECTS1) $(FLAGS)

$(TARGET2): $(OBJECTS2)
	$(CC) -o $@ $(OBJECTS2) $(FLAGS)

# Generic rule to compile .c files to .o files
# %.o represents any object file (.o file)
# %.c represents any source file (.c file)
# $< refers to the first prerequisite in the rule (.c file)
# $@ refers to the target of the rule (.o file)
%.o: %.c
	$(CC) -c $< $(FLAGS) -o $@

clean:
	rm -f $(TARGET1) $(TARGET2) *.o
