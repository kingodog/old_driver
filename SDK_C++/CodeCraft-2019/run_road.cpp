#include "run_road.h"

void run_all_road(Road *road, int road_num){
    int i;
    for(i = 0; i < road_num; i++)
    {
        dispatch_cars_on_road(&(road[i]))
    }
}

    int i, j;
    RoadQue *que = (RoadQue *)malloc(sizeof(RoadQue));
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
    for(i = 0; i < rows_num; i++){
        for(j = 0; j < columns_num; j++){
            que->lanes[i][j] = NULL;
        }
    }

    return que;
}

void destroy_road_que(RoadQue *que){
    free(que->lanes);
    que->lanes = NULL;
    free(que);
    que = NULL;
}

bool que_is_empty(RoadQue *que){
    return (que->head[ROW] == -1) || (que->head[CLM] == -1);
}

bool que_is_full(RoadQue *que){
    if((que->head[ROW] == -1) || (que->head[CLM] == -1)){
        return false;
    }else{
        return (que->tail[ROW] == 0) && (que->tail[CLM] == 0);
    }
}



bool enqueue(Road *road, Car *car, int real_speed, int dir){
    RoadQue * que;
    if(dir == FORWARD){
        que = road->forward;
    }else{
        que = road->back;
    }

    if(que_is_full(que)){
        printf("overflow!\n");
        return false;
    }

    que->tail[CLM] = road->length - real_speed;
    que->lanes[que->tail[ROW]][que->tail[CLM]] = car;
    car->status = END;

    if(que->tail[CLM] < que->head[CLM] || que_is_empty(que)){
        que->head[ROW] = que->tail[ROW];
        que->head[CLM] = que->tail[CLM];
    }

    que->tail[ROW] = (que->tail[ROW] + (que->tail[CLM] + 1) / road->length) % road->lanes_num;
    que->tail[CLM] = (que->tail[CLM] + 1) % road->length;

    return true;
}

bool get_waiting_head(Road *road, int dir, int *pos){
    RoadQue * que;
    if(dir == FORWARD){
        que = road->forward;
    }else{
        que = road->back;
    }

    if(que_is_empty(que)){
        pos[ROW] = -1;
        pos[CLM] = -1;
        return false;
    }

    int i, j;
    Car *tmp;
    for(j = que->head[CLM]; j < road->length; j++){
        for(i = que->head[ROW]; i < road->lanes_num; i++){
            tmp = que->lanes[i][j];
            if(!tmp){
                continue;
            }
            if(tmp->status == WAIT){
                pos[ROW] = i;
                pos[CLM] = j;
                return true;
            }
        }
    }
}

Car *dequeue(Road *road, int dir){
    RoadQue * que;
    if(dir == FORWARD){
        que = road->forward;
    }else{
        que = road->back;
    }

    if(que_is_empty(que)){
        printf("underflow!\n");
        return NULL;
    }

    int pos[2];
    if(!get_waiting_head(road, FORWARD, pos)){
        return NULL;
    }
    Car *tmp = que->lanes[pos[ROW]][pos[CLM]];
    que->lanes[pos[ROW]][pos[CLM]] = NULL;

    int row = que->head[ROW];
    int column = que->head[CLM];
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

void pass_through(RoadQue *que, int curr_row, int curr_column, int columns_num, int real_speed){
    if(curr_row == que->tail[ROW] && curr_column == (columns_num - 1)){
        que->tail[CLM] = curr_column - real_speed + 1;
    }

    Car *tmp = que->lanes[curr_row][curr_column];
    que->lanes[curr_row][curr_column] = NULL;
    curr_column -= real_speed;
    que->lanes[curr_row][curr_column] = tmp;
    tmp->status = END;

    if(curr_column < que->head[CLM] || (curr_column == que->head[CLM] && curr_row < que->head[ROW])){
        que->head[ROW] = curr_row;
        que->head[CLM] = curr_column;
    }
}

inline int get_min(int a, int b){
    return (a < b) ? a : b;
}

void dispatch_cars_on_road(Road *road){
    Car *prev, *curr;
    int block, speed;
    int i, j, k;
    for(k = 0; k <= road->bothway; k++){
        RoadQue * que = (k == 0) ? road->forward : road->back;
        for(i = 0; i < road->lanes_num; i++){
            block = -1;
            prev = NULL;
            for(j = 0; j < road->length; j++){
                curr = que->lanes[i][j];
                if(!curr){
                    continue; 
                }

                speed = get_min(get_min(road->limit, curr->speed), j - block - 1);

                if(!prev){
                    if((j - block - 1) < (get_min(road->limit, curr->speed))){
                        curr->status = WAIT;
                        block = j;
                    }else{
                        pass_through(que, i, j, road->length, speed);
                        block = j - speed;
                    }
                }else{
                    if((j - block - 1) < (get_min(road->limit, curr->speed)) && prev->status == WAIT){
                        curr->status = WAIT;
                        block = j;
                    }else{
                        pass_through(que, i, j, road->length, speed);
                        block = j - speed;
                    }
                }

                prev = curr;
            }
        }
    }
}

//测试
// int main(){
//     Road *road = (Road *)malloc(sizeof(Road));
//     road->length = 10;
//     road->lanes_num = 3;
//     road->limit = 4;
//     road->bothway = 0;
//     RoadQue *que = init_road_que(road->length, road->lanes_num);
//     road->forward = que;
//     road->back = NULL;

//     Car *car1 = (Car *)malloc(sizeof(Car));
//     car1->id = 1;
//     car1->speed = 1;
//     Car *car2 = (Car *)malloc(sizeof(Car));
//     car2->id = 2;
//     car2->speed = 2;
//     Car *car3 = (Car *)malloc(sizeof(Car));
//     car3->id = 3;
//     car3->speed = 3;
//     Car *car4 = (Car *)malloc(sizeof(Car));
//     car4->id = 4;
//     car4->speed = 4;
//     Car *car5 = (Car *)malloc(sizeof(Car));
//     car5->id = 5;
//     car5->speed = 5;
//     Car *car6 = (Car *)malloc(sizeof(Car));
//     car6->id = 6;
//     car6->speed = 6;
//     Car *car7 = (Car *)malloc(sizeof(Car));
//     car7->id = 7;
//     car7->speed = 7;
//     Car *car8 = (Car *)malloc(sizeof(Car));
//     car8->id = 8;
//     car8->speed = 8;

//     //空路进一辆车
//     assert(que_is_empty(road->forward));
//     if(que_is_empty(road->forward)){
//         printf("que is empty\n");
//     }
//     enqueue(road, car1, 1, FORWARD);
//     assert(road->forward->lanes[0][9] == car1);
//     if(road->forward->lanes[0][9] == car1){
//         printf("(0,9) has car1\n");
//     }
//     //进第二辆�
//     enqueue(road, car2, 2, FORWARD);
//     assert(road->forward->lanes[1][8] == car2);
//     if(road->forward->lanes[1][8] == car2){
//         printf("(1,8) has car2\n");
//     }
//     //进第三、四、五、六、七辆车
//     enqueue(road, car3, 1, FORWARD);
//     enqueue(road, car4, 4, FORWARD);
//     enqueue(road, car5, 3, FORWARD);
//     enqueue(road, car6, 2, FORWARD);
//     enqueue(road, car7, 1, FORWARD);
//     assert(road->forward->lanes[1][9] == car3);
//     if(road->forward->lanes[1][9] == car3){
//         printf("(1,9) has car3\n");
//     }
//     assert(road->forward->lanes[2][6] == car4);
//     if(road->forward->lanes[2][6] == car4){
//         printf("(2,6) has car4\n");
//     }
//     assert(road->forward->lanes[2][7] == car5);
//     if(road->forward->lanes[2][7] == car5){
//         printf("(2,7) has car5\n");
//     }
//     assert(road->forward->lanes[2][8] == car6);
//     if(road->forward->lanes[2][8] == car6){
//         printf("(2,8) has car6\n");
//     }
//     assert(road->forward->lanes[2][9] == car7);
//     if(road->forward->lanes[2][9] == car7){
//         printf("(2,9) has car7\n");
//     }
//     //进第八辆车失�
//     enqueue(road, car8, 8, FORWARD);
//     //行进并标记状�
//     while(road->forward->lanes[road->forward->head[ROW]][road->forward->head[CLM]]->status != WAIT){
//         dispatch_cars_on_road(road);
//         for(int i = 0; i < road->lanes_num; i++){
//             for(int j = 0; j < road->length; j++){
//                 if(!road->forward->lanes[i][j]){
//                     continue;
//                 }
//                 printf("(%d,%d) has car%d\n", i, j, road->forward->lanes[i][j]->id);
//             }
//         }
//     }
//     //出队
//     for(int j = 0; j < road->length; j++){
//         for(int i = 0; i < road->lanes_num; i++){
//             if(!road->forward->lanes[i][j]){
//                 continue;
//             }
//             if(road->forward->lanes[i][j]->status == WAIT){
//                 Car *car_out = dequeue(road, FORWARD);
//                 Car *car_head = road->forward->lanes[road->forward->head[ROW]][road->forward->head[CLM]];
//                 printf("car%d is out\n", car_out->id);
//                 printf("car%d is head now\n", car_head->id);
//             }
//         }
//     }
    


//     destroy_road_que(road->forward);
//     free(road);
//     road = NULL;

//     return 0;
// }

// int get_real_speed(Road *road, Car *car, int curr_column, int block){
//     return MIN(MIN(road->limit, car->speed), curr_column - block - 1);
// }

// void set_head(Road *road){
//     int i, j, k;
//     for(k = 0; k <= road->bothway; k++){
//         RoadQue * que = (k == 0) ? road->forward : road->back;
//         for(j = 0; j < road->length; j++){
//             for(i = 0; i < road->lanes_num; i++){
//                 if(que->lanes[i][j]){
//                     que->head[ROW] = i;
//                     que->head[CLM] = j;
//                     return; 
//                 }
//             }
//         }
        
//         if(j >= road->length || i >= road->lanes_num){
//             que->head[ROW] = -1;
//             que->head[CLM] = -1;
//         }
//     }
// }

// void set_tail(Road *road){
//     int i, j, k;
//     for(k = 0; k <= road->bothway; k++){
//         RoadQue * que = (k == 0) ? road->forward : road->back;
//         for(i = 0; i < road->lanes_num; i++){
//             for(j = road->length - 1; j >= 0 ; j--){
//                 if(j == road->length - 1){
//                     que->tail[ROW] = (i + 1) % road->lanes_num;
//                     que->tail[CLM] = (j + 1) % road->length;
//                     break;
//                 }

//                 if(que->lanes[i][j]){
//                     que->tail[ROW] = i;
//                     que->tail[CLM] = (j + 1) % road->length;
//                     return; 
//                 }
//             }
//         }
//     }
// }