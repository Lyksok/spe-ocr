#ifndef READ_DATA_H 
#define READ_DATA_H 

#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "convertion.h"
#include "neural_structures.h"
#include "setup.h"

TrainingData *ReadDirectory(const char *directory);
TrainingData *ParseDirectory();

#endif /* READ_DATA_H */
