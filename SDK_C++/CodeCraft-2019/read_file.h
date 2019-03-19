#ifndef _READ_FILE_H_
#define _READ_FILE_H_


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include "car.h"
#include "run_cross.h"
#include "run_road.h"
#include <hash_map>



#ifndef _ROAD_MAP_H_
#define _ROAD_MAP_H_

namespace std
{
    using namespace __gnu_cxx;
}

using namespace std;


#endif

#define CAR_PATH        "./SDK_C++/config/car.txt"
#define CROSS_PATH      "./SDK_C++/config/cross.txt"
#define ROAD_PATH       "./SDK_C++/config/road.txt"
#define DEFEATED        ((int)-1)
#define SUCCESSFUL      ((int)0)

#define NOCAR           (NULL)

#define UNBORN          (0)
#define READY           (1)
#define WAIT            (2)
#define END             (3)
#define ARRIVE          (4)





void get_imformation(int *car_num, int *cross_num, int *road_num, Car **car, 
        Cross **cross, Road **road,char *car_path, char *cross_path, char *road_path);
void get_cross_imformation(char *path,int *cross_num, Cross **cross);
void get_car_imformation(char *path,int *car_num, Car **car,int *road_num);
void get_road_imformation(char *path,int *road_num, Road **road);
int get_next_int(char **str, int *num);
void new_a_road_road_que(Road *road);
void sort_car_by_speed(Car *car, int car_num);

#endif