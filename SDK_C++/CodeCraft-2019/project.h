#ifndef _PROJECT_H_
#define _PROJECT_H_

#include <stdio.h>
#include <stdlib.h>
#include "read_file.h"
#include "topology.h"

void project_car(int car_num, int cross_num, int road_num, Car *car, Cross *cross, Road *road);
int ** get_precursor_matrix_floyd(int **weight_matrix, int cross_num);
int **new_a_int_matrix(int n);
void free_a_matrix(int **matrix, int n);

void reset_flow(Road *road, int road_num);
void reset_back_flow(Road *road, int road_num);
void reset_forward_flow(Road *road, int road_num);
#endif