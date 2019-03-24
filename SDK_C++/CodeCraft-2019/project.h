#ifndef _PROJECT_H_
#define _PROJECT_H_

#include <stdio.h>
#include <stdlib.h>
#include "topology.h"
#include "run_road.h"
#include "car.h"
#include "run_cross.h"
#include <hash_map>

#define SIGNED  1
#define UNSIGN  0

void project_car(int car_num, int cross_num, int road_num, Car *car, Cross *cross, Road *road);
void project_all_waiting_car(Road *road, int road_num, Cross *cross, int cross_num);
void project_a_road_waiting_car(Road *this_road, Road *all_road, int road_num, Cross *cross, int cross_num);
int get_next_road(int start, int end, Road *road, Cross *cross, int road_num, int cross_num, int speed);
int ** get_precursor_matrix_floyd(int **weight_matrix, int cross_num);
int **new_a_int_matrix(int n);
void free_a_matrix(int **matrix, int n);
void reset_all_pre_flow(Road *road, int road_num);

#endif