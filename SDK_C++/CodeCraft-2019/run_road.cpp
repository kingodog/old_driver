#include "run_road.h"

Road_que *init_road_que(int columns_num, int rows_num){
    int i;
    Road_que *road = (Road_que *)malloc(sizeof(Road_que));
    Car ***lanes = (Car ***)malloc(sizeof(Car **) * rows_num);
    assert(road && lanes);

    for(i = 0; i < rows_num; i++){
        lanes[i] = (Car **)malloc(sizeof(Car*) * columns_num);
        assert(lanes[i]);
    }

    road->head = -1;
    road->tail = 0;
    road->lanes = lanes;

    return road;
}

void destroy_road_que(Road_que *road){
    free(road->lanes);
    road->lanes = NULL;
    free(road);
    road = NULL;
}

bool que_is_empty(Road_que *que){
    return que->head == -1;
}

bool que_is_full(Road_que *que){
    return que->tail == que->head;
}

bool enqueue(Road *road, Car *car, int real_speed, int dir){
    Road_que * que;
    if(dir == 1){
        que = road->forward;
    }else{
        que = road->back;
    }

    if(que_is_full(que)){
        printf("overflow!\n");
        return false;
    }
    
    que->lanes[que->tail / road->length][road->length - real_speed] = car;
    que->tail = (que->tail / road->length) * road->length + road->length - real_speed;
    if(que->head == -1){
        que->head = que->tail;
    }
    que->tail = (que->tail + 1) % (road->length * road->lanes_num);

    return true;
}

Car *dequeue(Road *road, int dir){
    Road_que * que;
    if(dir == 1){
        que = road->forward;
    }else{
        que = road->back;
    }

    if(que_is_empty(que)){
        printf("underflow!\n");
        return NULL;
    }

    int row = que->head / road->length;
    int column = que->head % road->length;
    Car *tmp = que->lanes[row][column];
    que->lanes[row][column] = NULL;

    while(!que->lanes[row][column] && que->head != que->tail){
        row = (row + 1) % road->lanes_num;
        if(!row){
            column++;
        }
        que->head = row * road->length + column;
    }
    
    if(que->head == que->tail){
        que->head = -1;
        que->tail = 0;
    }

    return tmp;
}

void pass_through(Road_que *que, int curr_row, int curr_column, int real_speed){
    Car *tmp = que->lanes[curr_row][curr_column];
    que->lanes[curr_row][curr_column - real_speed] = tmp;
    que->lanes[curr_row][curr_column] = NULL;
}

int get_real_speed(Road *road, Car *car, int curr_column, int block){
    return MIN(MIN(road->limit, car->speed), curr_column - block);
}

// void run_cars_on_road(Road *road){
//     int block = 0;
//     int i, j;
//     while(road->forward->head){

//     }
// }