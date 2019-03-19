#include "run_road.h"

Road_que *init_road_que(int columns_num, int rows_num){
    int i;
    Road_que *que = (Road_que *)malloc(sizeof(Road_que));
    Car ***lanes = (Car ***)malloc(sizeof(Car **) * rows_num);
    assert(que && lanes);

    for(i = 0; i < rows_num; i++){
        lanes[i] = (Car **)malloc(sizeof(Car*) * columns_num);
        assert(lanes[i]);
    }

    que->head[ROW] = -1;
    que->head[CLM] = -1;
    que->tail[ROW] = 0;
    que->tail[CLM] = 0;
    que->lanes = lanes;

    return que;
}

void destroy_road_que(Road_que *que){
    free(que->lanes);
    que->lanes = NULL;
    free(que);
    que = NULL;
}

bool que_is_empty(Road_que *que){
    return (que->head[ROW] == -1) || (que->head[CLM] == -1);
}

bool que_is_full(Road_que *que){
    return (que->tail[ROW] == que->head[ROW]) && (que->tail[CLM] == que->head[CLM]);
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
    
    que->lanes[que->tail[ROW]][road->length - real_speed] = car;

    if(que_is_empty(que)){
        que->head[ROW] = que->tail[ROW];
        que->head[CLM] = que->tail[CLM];
    }

    que->tail[ROW] += (que->tail[CLM] + 1) / road->length * que->tail[ROW];
    que->tail[CLM] = (que->tail[CLM] + 1) % road->length;

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

    int row = que->head[ROW];
    int column = que->head[CLM];
    Car *tmp = que->lanes[row][column];
    que->lanes[row][column] = NULL;

    while(!que->lanes[row][column] && (row != que->tail[ROW] || column != que->tail[CLM])){
        row = (row + 1) % road->lanes_num;
        if(!row){
            column = (column + 1) % road->length;
        }
    }
    que->head[ROW] = row;
    que->head[CLM] = column;
    
    if(que->head[ROW] == que->tail[ROW] && que->head[CLM] == que->tail[CLM]){
        que->head[ROW] = -1;
        que->head[CLM] = -1;
        que->tail[ROW] = 0;
        que->tail[CLM] = 0;
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

void set_head(Road *road){
    int i, j, k;
    for(k = 0; k <= road->bothway; k++){
        Road_que * que = (k == 0) ? road->forward : road->back;
        for(j = 0; j < road->length; j++){
            for(i = 0; i < road->lanes_num; i++){
                if(que->lanes[i][j]){
                    que->head[ROW] = i;
                    que->head[CLM] = j;
                    return; 
                }
            }
        }
    }
}

void set_tail(Road *road){
    int i, j, k;
    for(k = 0; k <= road->bothway; k++){
        Road_que * que = (k == 0) ? road->forward : road->back;
        for(i = 0; i < road->lanes_num; i++){
            for(j = road->length - 1; j >= 0 ; j--){
                if(que->lanes[i][j]){
                    que->tail[ROW] = i;
                    que->tail[CLM] = j;
                    return; 
                }
            }
        }
    }
}

void set_cars_status(Road *road){
    Car *prev, *curr;
    int block, speed;
    int i, j, k;
    for(k = 0; k <= road->bothway; k++){
        Road_que * que = (k == 0) ? road->forward : road->back;
        for(i = 0; i < road->lanes_num; i++){
            block = 0;
            prev = NULL;
            for(j = 0; j < road->length; j++){
                curr = que->lanes[i][j];
                if(!curr){
                    continue; 
                }

                speed = MIN(MIN(road->limit, curr->speed), j - block);

                if(!prev){
                    if((j - block) < MIN(road->limit, curr->speed)){
                        curr->status = WAIT;
                    }else{
                        pass_through(que, i, j, speed);
                        curr->status = END;
                    }
                }

                if((j - block) < MIN(road->limit, curr->speed) && prev->status == WAIT){
                    curr->status = WAIT;
                }else{
                    pass_through(que, i, j, speed);
                    curr->status = END;
                }

                block = j;
                prev = curr;
            }
        }
    }

    set_head(road);
    set_tail(road);
}