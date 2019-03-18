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
}Cross;


#endif