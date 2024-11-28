CC = gcc

<<<<<<< HEAD
SOURCE = segmentation.c cluster-detection.c bounding-box.c detection.c histograms.c main.c ../image_processing/binarization/binarizing.c ../image_processing/utils/histograms.c ../image_processing/thresholding/global.c ../image_processing/utils/converting.c ../image_processing/thresholding/adaptative.c ../image_processing/thresholding/sauvola.c ../image_processing/utils/pixel_utils.c ../image_processing/utils/helper.c
TARGET = det-seg
OBJECTS = ${SOURCE:.c=.o}
=======
SOURCE = segmentation.c cluster-detection.c bounding-box.c detection.c histograms.c main.c ../binarization/binarizing.c ../binarization/histograms.c ../binarization/global_thresholding.c ../binarization/converting.c ../binarization/adaptative_thresholding.c
TARGET = det-seg
OBJECTS = segmentation.o cluster-detection.o bounding-box.o detection.o histograms.o main.o ../binarization/binarizing.o ../binarization/histograms.o ../binarization/global_thresholding.o ../binarization/converting.o ../binarization/adaptative_thresholding.o
>>>>>>> c2c26dc (Move detection and segmentation to image processing)
LIBS = -lSDL2 -lSDL2_image -lm
FLAGS = -Wall -Wextra -O2 -fsanitize=address

all: $(TARGET)

$(TARGET): $(OBJECTS)
<<<<<<< HEAD
	$(CC) -o $(TARGET) $(SOURCE) $(FLAGS) $(LIBS)

$(OBJECTS):
=======
	$(CC) -o $@ $(OBJECTS) $(FLAGS) $(LIBS)

# Generic rule to compile .c files to .o files
# %.o represents any object file (.o file)
# %.c represents any source file (.c file)
# $< refers to the first prerequisite in the rule (.c file)
# $@ refers to the target of the rule (.o file)
%.o: %.c
	$(CC) -c $< $(FLAGS) -o $@
>>>>>>> c2c26dc (Move detection and segmentation to image processing)

clean:
	rm -f $(TARGET) $(OBJECTS)
