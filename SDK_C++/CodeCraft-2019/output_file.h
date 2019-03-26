#ifndef _OUTPUT_FILE_H_
#define _OUTPUT_FILE_H_

#include <stdio.h>
#include <stdlib.h>
#include "car.h"

#define OUTPUT_PATH       (char *)"./SDK_C++/config/output.txt"



void output_file(char *path, Car *car, int car_num);

#endif