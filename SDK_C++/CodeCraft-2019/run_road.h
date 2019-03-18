#ifndef _RUN_ROAD_H_
#define _RUN_ROAD_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include <car.h>

typedef struct {
    int head;
    int tail;
    Car ***room;
} road_que;

typedef struct {
    int id;
    int length;
    int limit;
    int lanes_num;
    int cross_id_start;
    int cross_id_end;
    int bothway;

    int capacity;
    int forward_flow_num;
    int back_flow_num;

    road_que *forward;
    road_que *back;
} Road;

#endif