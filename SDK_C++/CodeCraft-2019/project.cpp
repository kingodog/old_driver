#include "project.h"

extern unsigned int time;
extern hash_map<int, Road> road_map;
extern hash_map<int, Cross> cross_map;

void project_car(int car_num, int cross_num, int road_num, Car *car, Cross *cross, Road *road){
    int no_car = 0;
    int surplus_unborn_car_num = car_num;

    put_car(car, road, cross, ture, true);//第一次特殊，先加车

    while(no_car == 0){       
       run_all_road(road, road_num);
       project_all_waiting_car(road, road_num, cross, cross_num);

       no_car = put_car(car, road, cross, ture, true, &surplus_unborn_car_num)
       
    }

    while(running_car_num != 0){
        
    }
    return;
}



void project_all_waiting_car(Road *road, int road_num, Cross *cross, int cross_num){
    int i;
    for(i = 0; i < road_num; i++)
    {
        project_a_road_waiting_car(&road[i]);
    }
}

void project_a_road_car(Road *this_road, Road *all_road, int road_num, Cross *cross, int cross_num){
    int i, j;
    Car ***que = this_road->forward->lanes;


    for( i = 0; i < this_road->lanes_num; i++)
    {
        for( j = 0; j < this_road->length; j++)
        {
            if(que[i][j] !=NULL){
                if(que[i][j]->status == WAIT && i < que[i][j]->speed){
                    que[i][j]->next_step = get_next_road(this_road->cross_id_end, que[i][j]->end, all_road, cross, road_num, cross_num, que[i][j]->speed);
                    if(que[i][j]->next_step == -1){             //到达目的地
                        que[i][j]->next_dir = -1;
                        this_road->pre_forward_surplus_flow -=que[i][j]->speed;
                        continue;
                    } else {
                        que[i][j]->next_dir = get_direction_by_road_id(cross_map[this_road->cross_id_end], this_road->id, que[i][j]->next_step);
                        this_road->pre_forward_surplus_flow -=que[i][j]->speed;
                        road_map[que[i][j]->next_step].
                    }   
                    
                } else {
                    break;                  //该条道路的后车就不用检测
                }
            }
        }
    }


    if(road->bothway == 1){
        que = this_road->back->lanes;



    }
    
}


int get_next_road(int start, int end, Road *road, Cross *cross, int road_num, int cross_num, int speed){
    int **weight_matrix = build_weight_matrix_by_capacity(cross, road, cross_num, road_num, speed);
    //todo




    free_a_matrix(weight_matrix);
}










int ** get_precursor_matrix_floyd(int **weight_matrix, int cross_num){
    int ***iteration_matrix;
    int ***precursor_matrix; 
    int i, j, k, l, m;
    iteration_matrix = (int ***)malloc(sizeof(int**)*(cross_num+1));        //比结点多一个（第0个）
    precursor_matrix = (int ***)malloc(sizeof(int**)*(cross_num+1));  
    for(k = 0; k < cross_num + 1; k++){
        iteration_matrix[k] = new_a_int_matrix(cross_num);  
        precursor_matrix[k] = new_a_int_matrix(cross_num);  
    }
    
    for(i = 0; i < cross_num; i++){
        for(j=0; j < cross_num; j++){
            (iteration_matrix[0])[i][j] = weight_matrix[i][j];
            if((i == j) || (weight_matrix[i][j] == INFINITY_INT)){
                precursor_matrix[0][i][j] = NIL;
            } else {
                precursor_matrix[0][i][j] = i;
            }
        }
    }

    for(k = 1; k < cross_num+1; k++){
        for(i = 0; i < cross_num; i++){
            for(j = 0; j < cross_num; j++){
                iteration_matrix[k][i][j] = get_min(iteration_matrix[k-1][i][j], (iteration_matrix[k-1][i][k-1] + iteration_matrix[k-1][k-1][j]));
                if(iteration_matrix[k-1][i][j] <= (iteration_matrix[k-1][i][k-1] + iteration_matrix[k-1][k-1][j])){
                    precursor_matrix[k][i][j] = precursor_matrix[k-1][i][j];
                } else {
                    precursor_matrix[k][i][j] = precursor_matrix[k-1][k-1][j];
                }
            }   
        }   
    }   

    for(k = 0; k < cross_num; k++){
        free_a_matrix(iteration_matrix[k], cross_num);
        free_a_matrix(precursor_matrix[k], cross_num);
    }

    free_a_matrix(iteration_matrix[cross_num], cross_num);
    free(iteration_matrix);
    // free(precursor_matrix);

    return precursor_matrix[cross_num];
}

int **new_a_int_matrix(int n){
    int **matrix;
    int i;
    matrix = (int **)malloc(sizeof(int*)*n);  
    for(i = 0; i < n; i++){
        matrix[i] = (int*)malloc(sizeof(int)*n); 
    }
    return matrix;
}

void free_a_matrix(int **matrix, int n){
    int i;
    for(i = 0; i < n; i++){
        free(matrix[i]); 
    }
    free(matrix);
}

void reset_all_pre_flow(Road *road, int road_num){
    int i;
    for(i = 0; i < road_num; i++){
        road[i].pre_forward_surplus_flow = road[i].forward_surplus_flow;
        road[i].pre_back_surplus_flow = road[i].back_surplus_flow;
    }
}

// void reset_flow(Road *road, int road_num){
//     int i;
//     for(i = 0; i < road_num; i++){
//         road[i].forward_surplus_flow = 0;
//         road[i].back_surplus_flow = 0;
//     }
// }

// void reset_back_flow(Road *road, int road_num){
//     int i;
//     for(i = 0; i < road_num; i++){
//         road[i].back_flow_num = 0;
//     }
// }

// void reset_forward_flow(Road *road, int road_num){
//     int i;
//     for(i = 0; i < road_num; i++){
//         road[i].forward_flow_num = 0;
//     }
// }

// int get_next_road(int start, int end, Cross *cross){                //todo
//     return 1;
// }