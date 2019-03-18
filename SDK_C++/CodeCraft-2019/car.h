#ifndef _RUN_CAR_H_
#define _RUN_CAR_H_

typedef struct 
{
    int car_id;
    int start_time;
    int road_num;
    int *road;
}CarProject;

typedef struct 
{
    int id;
    int start;
    int end; 
    int speed;
    int start_time;

    int status;

    CarProject *project;
}Car;

#endif
