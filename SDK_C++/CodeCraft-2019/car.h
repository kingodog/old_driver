#ifndef _RUN_CAR_H_
#define _RUN_CAR_H_

#define  CAR_MAX_RECORD (100)


#define END             (3)
#define ARRIVE          (4)

typedef struct 
{
    int car_id;
    int start_time;
    int road_num;
    int road[CAR_MAX_RECORD];
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
