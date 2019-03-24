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

#define  RUNNING_MAX_NUM      (500)

typedef struct 
{
    int id;
    int total_road;
    int road_id[4];
    Road *road[4];
    int road_id_sorted[4];

    RoadQue *road_dir[4];   //代表的是该路口管理的车道
    RoadQue *road_dir_out[4];   //代表的是该路口可以注入的车道
}Cross;

void put_car(Car *car, Road *road, Cross *cross);
int get_direction_by_road_id(Cross cross, int start_id, int end_id);
void car_new_a_project_road(Car *car, int road_id);
void run_all_cross(Cross *cross, int cross_num);
void run_a_cross(Cross *cross);
void run_a_road(Cross *cross, RoadQue *way, Road *road, int *end_flag);
void adjust_a_lane(int start, int end, Car **lane, int limit_speed);
Car *get_left_road_first_car(Cross * corss, int self_road_id);
Car *get_right_road_first_car(Cross * corss, int self_road_id);
Car *get_across_road_first_car(Cross * corss, int self_road_id)
#endif