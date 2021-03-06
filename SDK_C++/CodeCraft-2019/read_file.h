#ifndef _READ_FILE_H_
#define _READ_FILE_H_


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include "car.h"
#include "run_cross.h"
#include "run_road.h"
#include "topology.h"
#include <hash_map>



#ifndef _ROAD_MAP_H_
#define _ROAD_MAP_H_

namespace std
{
    using namespace __gnu_cxx;
}

using namespace std;


#endif

#define CAR_PATH        (char *)"./SDK_C++/config/car.txt"
#define CROSS_PATH      (char *)"./SDK_C++/config/cross.txt"
#define ROAD_PATH       (char *)"./SDK_C++/config/road.txt"
#define DEFEATED        ((int)-1)
#define SUCCESSFUL      ((int)0)

#define MAX_SPEED      ((int)17)                //  speed+1  才行   切记

#define NOCAR           (NULL)

void get_imformation(int *car_num, int *cross_num, int *road_num, Car **car, 
        Cross **cross, Road **road, const char *car_path, const char *cross_path, const char *road_path);
void get_cross_imformation(const char *path, int *cross_num, Cross **cross);
void get_car_imformation(const char *path,int *car_num, Car **car,int *road_num);
void get_road_imformation(const char *path,int *road_num, Road **road);
int get_next_int(char **str, int *num);
void new_a_road_road_que(Road *road);
void sort_car_by_speed_and_creat_list(Car *car, int car_num);
void delete_car_from_list(CarList **p);
void sort_cross_road_id(Cross *cross);
void exchange_int_num(int *a, int *b);


void merge_roads(Road *array, int left, int mid, int right);
void merge_sort_roads(Road *array, int left, int right);
void merge_crosses(Cross *array, int left, int mid, int right);
void merge_sort_crosses(Cross *array, int left, int right);
void merge_cars(Car *array, int left, int mid, int right);
void merge_sort_cars(Car *array, int left, int right);

void build_cross_num_to_road_num_matrix(int cross_num, int road_num, Cross *cross, Road *road);
#endif