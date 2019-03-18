#ifndef _OUTPUT_FILE_H_
#define _OUTPUT_FILE_H_

#include <stdio.h>
#include <stdlib.h>
#include "car.h"

#define OUTPUT_PATH       "./SDK_C++/config/output.txt"



void output_file(char *path, CarProject *carproject, int car_num);

#endif