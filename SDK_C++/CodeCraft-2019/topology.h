#ifndef _TOPOLOGY_H_
#define _TOPOLOGY_H_

#include <stdio.h>
#include <stdlib.h>
#include "read_file.h"
// #define NO_CONNECT              (0x1<<30)
#define NO_CONNECT              (-1)

float ** build_weight_matrix(Cross *cross, Road *road, int cross_num, int  road_num,int speed);
float get_road_weight(int start_id, int end_id, Road *road, int road_num, int speed);
int get_min(int a, int b);

#endif