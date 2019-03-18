#include "run_road.h"

road_que *init_road_que(int length, int width){
    int i;
    road_que *road = (road_que *)malloc(sizeof(road_que));
    Car ***lanes = (Car ***)malloc(sizeof(Car **) * width);
    assert(road && lanes);

    for(i = 0; i < width; i++){
        lanes[i] = (Car **)malloc(sizeof(Car*) * length);
        assert(lanes[i]);
    }

    road->head = -1;
    road->tail = 0;
    road->room = lanes;

    return road;
}

void destroy_road_que(road_que *road){
    free(road->room);
    road->room = NULL;
    free(road);
    road = NULL;
}

bool is_empty(road_que *que){
    return que->head == -1;
}

bool is_full(road_que *que){
    return que->tail == que->head;
}

bool enqueue(Road *road, Car *car, int speed, int dir){
    road_que * que;
    if(dir == 1){
        que = road->forward;
    } else {
        que = road->back;
    }
    if(is_full(road->forward)){
        printf("overflow!\n");
        return false;
    }
    
    road->forward->room[road->forward->tail / road->length][road->length - speed] = car;
    if(road->forward->head == -1){
        road->forward->head = road->forward->tail;
    }
    road->forward->tail = (road->forward->head + 1) % (road->length * road->lanes_num);

    return true;
}

Car *dequeue(Road *road){
    if(is_empty(road->forward)){
        printf("underflow!\n");
        return NULL;
    }

    Car *tmp = road->forward->room[road->forward->head / road->length][road->forward->head % road->length];
    road->forward->head = (road->forward->head + 1) % (road->length * road->lanes_num);
    if(road->forward->head == road->forward->tail){
        road->forward->head = -1;
        road->forward->tail = 0;
    }

    return tmp;
}