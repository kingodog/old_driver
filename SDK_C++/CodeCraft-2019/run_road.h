#ifndef _RUN_ROAD_H_
#define _RUN_ROAD_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include "car.h"

#define FORWARD         ((int)1)
#define BACK            ((int)-1)
#define ROW             ((int)0)
#define CLM             ((int)1)

#define COME_ON             ((int)1)
#define GET_OUT             ((int)2)
#define PLEASE_WAIT         ((int)3)

typedef struct {
    int head[2];
    int tail[2];
    Car ***lanes;
} RoadQue;

typedef struct {
    int id;
    int length;
    int limit;
    int lanes_num;
    int cross_id_start;
    int cross_id_end;
    int bothway;

    int capacity;
    int forward_surplus_flow;
    int back_surplus_flow;

    int pre_forward_surplus_flow;
    int pre_back_surplus_flow;

    RoadQue *forward;
    RoadQue *back;
} Road;


void run_all_road(Road *road, int road_num);
RoadQue *init_road_que(int columns_num, int rows_num);
void destroy_road_que(RoadQue *que);
bool que_is_empty(RoadQue *que);
bool que_is_full(RoadQue *que);
bool enqueue(Road *road, Car *car, int real_speed, int dir);
bool get_waiting_head(Road *road, int dir, int *pos);
Car *dequeue(Road *road, int dir);
void pass_through(RoadQue *que, int curr_row, int curr_column, int columns_num, int real_speed);
inline int get_min(int a, int b){
    return ((a < b) ? a : b);
}
void dispatch_cars_on_road(Road *road);

int cross_through(Road *road, Car *car, int distance, RoadQue * que);
// int get_real_speed(Road *road, Car *car, int curr_column, int block);
// void set_head(Road *road);
// void set_tail(Road *road);

#endif