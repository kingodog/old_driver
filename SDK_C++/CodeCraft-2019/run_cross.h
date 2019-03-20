#ifndef _RUN_CROSS_H_
#define _RUN_CROSS_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include "run_road.h"

#define  STRAIGHT   (0)
#define  LEFT       (1)
#define  RIGHT      (2)

typedef struct 
{
    int id;
    int road_id[4];
    int road_id_sorted[4];
}Cross;

void put_car(Car *car, Road *road, Cross *cross);
int get_direction_by_road_id(Cross cross, int start_id, int end_id);
void car_new_a_project_road(Car *car, int road_id);
void run_all_cross(Cross *cross, int cross_num);

#endif