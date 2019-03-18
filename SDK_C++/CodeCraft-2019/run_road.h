#ifndef _RUN_ROAD_H_
#define _RUN_ROAD_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include "car.h"
typedef struct 
{
    int id;
    int lenth;
    int limit;
    int ways;
    int cross_id_strat;
    int cross_id_end;
    int bothway;

    int capacity;
    int forward_flow_num;
    int back_flow_num;

    Car ***forward_content;
    Car ***back_content;
}Road;



#endif