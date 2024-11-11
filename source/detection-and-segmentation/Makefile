CC = gcc

SOURCE = segmentation.c cluster-detection.c bounding-box.c detection.c histograms.c main.c ../binarization/binarizing.c ../binarization/histograms.c ../binarization/global_thresholding.c ../binarization/converting.c ../binarization/local_thresholding.c
TARGET = det-seg
OBJECTS = segmentation.o cluster-detection.o bounding-box.o detection.o histograms.o main.o ../binarization/binarizing.o ../binarization/histograms.o ../binarization/global_thresholding.o ../binarization/converting.o ../binarization/local_thresholding.o
LIBS = -lSDL2 -lSDL2_image -lm
FLAGS = -Wall -Wextra -O2 -fsanitize=address

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(FLAGS) $(LIBS)

# Generic rule to compile .c files to .o files
# %.o represents any object file (.o file)
# %.c represents any source file (.c file)
# $< refers to the first prerequisite in the rule (.c file)
# $@ refers to the target of the rule (.o file)
%.o: %.c
	$(CC) -c $< $(FLAGS) -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)
