#ifndef _OUTPUT_FILE_H_
#define _OUTPUT_FILE_H_

#include <stdio.h>
#include <stdlib.h>
#include "read_file.h"

#define OUTPUT_PATH       "./SDK_C++/config/output.txt"

typedef struct 
{
    int car_id;
    int start_time;
    int road_num;
    int *road;
}CarProject;

void output_file(char *path, CarProject *carproject, int car_num);

#endif