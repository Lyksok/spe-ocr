CC = gcc

TARGET-TEST = test
TARGET = solver
OBJECTS-TEST = solver.o test.o
OBJECTS = solver.o read.o main.o
FLAGS = -Wall -Wextra -Werror

#all: $(TARGET-TEST) $(TARGET)
all: $(TARGET)

#$(TARGET-TEST): $(OBJECTS-TEST)
#	$(CC) -o $@ $(OBJECTS-TEST) $(FLAGS)

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(FLAGS)

# Generic rule to compile .c files to .o files
# %.o represents any object file (.o file)
# %.c represents any source file (.c file)
# $< refers to the first prerequisite in the rule (.c file)
# $@ refers to the target of the rule (.o file)
%.o: %.c
	$(CC) -c $< $(FLAGS) -o $@

clean:
	rm -f $(TARGET) $(OBJECTS) $(OBJECTS-TEST) $(TARGET-TEST)
