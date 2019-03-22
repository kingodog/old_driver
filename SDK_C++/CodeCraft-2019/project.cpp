#include "project.h"

extern unsigned int time;
extern hash_map<int, Cross> cross_map;

void project_car(int car_num, int cross_num, int road_num, Car *car, Cross *cross, Road *road){
        
    
    return;
}

//可以按照书上进行优化
int get_next_road(int start, int end, Road *road, Cross *cross, int road_num, int cross_num, int speed){
    int **weight_matrix = build_weight_matrix_by_capacity(cross, road, cross_num, road_num, speed);
    //todo
    int dist[cross_num], prev[cross_num], flag[cross_num];
    int i, j, k;
    //初始化
    for(i = 0; i < cross_num; i++){
        dist[i] = INFINITY_INT;
        prev[i] = NIL;
        flag[i] = SIGNED;
    }
    //找到start在cross数组中id，并初始化它的顶点属性
    int src_id = ((int)(&cross_map[start]) - (int)(cross))/sizeof(Cross);
    dist[src_id] = 0;
    flag[src_id] = UNSIGN;
    //遍历除了start顶点的其他顶点
    for(i = 0; i < cross_num - 1; i++){
        //找到未标记的顶点的最短估计中最小者
        int min = INFINITY_INT;
        for(j = 0; j < cross_num; j++){
            if(flag[j] == UNSIGN && dist[j] < min){
                min = dist[j];
                k = j;
            }
        }
        flag[k] = SIGNED;
        //松弛
        for(j = 0; j < cross_num; j++){
            int tmp = (weight_matrix[k][j] == INFINITY_INT ? INFINITY_INT : (min + weight_matrix[k][j]));//防止溢出
            if(tmp == INFINITY_INT){
                continue;
            }
            if(flag[j] == UNSIGN && tmp < dist[j]){
                dist[j] = tmp;
                prev[j] = k;
            }
        }
    }
    free_a_matrix(weight_matrix);

    //找到end在cross数组中id
    int end_id = ((int)(&cross_map[end]) - (int)(cross))/sizeof(Cross);
    while(!prev[end_id]){
        end_id = prev[end_id];
    }
    Cross next_cross = cross[end_id];
    //TODO:通过下一个路口得到下一条路
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

int ** get_prevursor_matrix_floyd(int **weight_matrix, int cross_num){
    int ***iteration_matrix;
    int ***prevursor_matrix; 
    int i, j, k, l, m;
    iteration_matrix = (int ***)malloc(sizeof(int**)*(cross_num+1));        //比结点多一个（第0个）
    prevursor_matrix = (int ***)malloc(sizeof(int**)*(cross_num+1));  
    for(k = 0; k < cross_num + 1; k++){
        iteration_matrix[k] = new_a_int_matrix(cross_num);  
        prevursor_matrix[k] = new_a_int_matrix(cross_num);  
    }
    
    for(i = 0; i < cross_num; i++){
        for(j=0; j < cross_num; j++){
            (iteration_matrix[0])[i][j] = weight_matrix[i][j];
            if((i == j) || (weight_matrix[i][j] == INFINITY_INT)){
                prevursor_matrix[0][i][j] = NIL;
            } else {
                prevursor_matrix[0][i][j] = i;
            }
        }
    }

    for(k = 1; k < cross_num+1; k++){
        for(i = 0; i < cross_num; i++){
            for(j = 0; j < cross_num; j++){
                iteration_matrix[k][i][j] = get_min(iteration_matrix[k-1][i][j], (iteration_matrix[k-1][i][k-1] + iteration_matrix[k-1][k-1][j]));
                if(iteration_matrix[k-1][i][j] <= (iteration_matrix[k-1][i][k-1] + iteration_matrix[k-1][k-1][j])){
                    prevursor_matrix[k][i][j] = prevursor_matrix[k-1][i][j];
                } else {
                    prevursor_matrix[k][i][j] = prevursor_matrix[k-1][k-1][j];
                }
            }   
        }   
    }   

    for(k = 0; k < cross_num; k++){
        free_a_matrix(iteration_matrix[k], cross_num);
        free_a_matrix(prevursor_matrix[k], cross_num);
    }

    free_a_matrix(iteration_matrix[cross_num], cross_num);
    free(iteration_matrix);
    // free(prevursor_matrix);

    return prevursor_matrix[cross_num];
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