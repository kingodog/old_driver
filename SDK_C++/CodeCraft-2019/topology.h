#ifndef _TOPOLOGY_H_
#define _TOPOLOGY_H_

#include <stdio.h>
#include <stdlib.h>
#include "read_file.h"
// #define NO_CONNECT              (0x1<<30)
#define NO_CONNECT              (-1)

int ** build_weight_matrix(Cross *cross, Road *road, int cross_num, int  road_num, int speed);
int get_road_weight(int start_id, int end_id, Road *road, int road_num, int speed);

int ** build_capacity_matrix_by_ways(Cross *cross, Road *road, int cross_num, int  road_num);
int get_road_capacity_by_ways(int start_id, int end_id, Road *road, int road_num);

int ** build_cost_matrix_by_length(Cross *cross, Road *road, int cross_num, int  road_num);
int get_road_cost_by_length(int start_id, int end_id, Road *road, int road_num);

int get_min(int a, int b);

#endif