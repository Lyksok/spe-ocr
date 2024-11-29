CC = gcc

SOURCE = segmentation.c cluster-detection.c bounding-box.c detection.c histograms.c main.c ../image_processing/binarization/binarizing.c ../image_processing/utils/histograms.c ../image_processing/thresholding/global.c ../image_processing/utils/converting.c ../image_processing/thresholding/adaptative.c ../image_processing/thresholding/sauvola.c ../image_processing/utils/pixel_utils.c ../image_processing/utils/helper.c
TARGET = det-seg
OBJECTS = ${SOURCE:.c=.o}
LIBS = -lSDL2 -lSDL2_image -lm
FLAGS = -Wall -Wextra -O2 -fsanitize=address

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGET) $(SOURCE) $(FLAGS) $(LIBS)

$(OBJECTS):

clean:
	rm -f $(TARGET) $(OBJECTS)
