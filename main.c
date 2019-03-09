#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include "read_file.h"

void main(){
    int car_num;
    int road_num;
    int cross_num;
    Car *car=NULL;
    Cross *cross=NULL;
    Road *road=NULL;
    get_imformation(&car_num, &cross_num, &road_num, &car, &cross, &road);
    return;
} 