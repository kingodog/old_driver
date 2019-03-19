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
#define MIN(a, b)       (a < b)?(a):(b)

typedef struct {
    int head[2];
    int tail[2];
    Car ***lanes;
} Road_que;

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

    Road_que *forward;
    Road_que *back;
} Road;

#endif