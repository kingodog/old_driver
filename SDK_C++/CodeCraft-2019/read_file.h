#ifndef _READ_FILE_H_
#define _READ_FILE_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

#define CAR_PATH        "./SDK_C++/config/car.txt"
#define CROSS_PATH      "./SDK_C++/config/cross.txt"
#define ROAD_PATH       "./SDK_C++/config/road.txt"
#define DEFEATED        ((int)-1)
#define SUCCESSFUL      ((int)0)

typedef struct 
{
    int id;
    int start;
    int end; 
    int speed;
    int start_time;
}Car;

typedef struct 
{
    int id;
    int road_id[4];
}Cross;

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
}Road;

void get_imformation(int *car_num, int *cross_num, int *road_num, Car **car, 
        Cross **cross, Road **road,char *car_path, char *cross_path, char *road_path);
void get_cross_imformation(char *path,int *cross_num, Cross **cross);
void get_car_imformation(char *path,int *road_num, Car **car);
void get_road_imformation(char *path,int *road_num, Road **road);
int get_next_int(char **str, int *num);

#endif