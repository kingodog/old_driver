#include "project.h"

extern unsigned int time;
extern Car *car_head;
void project_car(int car_num, int cross_num, int road_num, Car *car, Cross *cross, Road *road){
    //sort()
    // car_head = &(car[0]);
    // put_car(car_head); 
    return;
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

void reset_flow(Road *road, int road_num){
    int i;
    for(i = 0; i < road_num; i++){
        road[i].forward_flow_num = 0;
        road[i].back_flow_num = 0;
    }
}

void reset_back_flow(Road *road, int road_num){
    int i;
    for(i = 0; i < road_num; i++){
        road[i].back_flow_num = 0;
    }
}

void reset_forward_flow(Road *road, int road_num){
    int i;
    for(i = 0; i < road_num; i++){
        road[i].forward_flow_num = 0;
    }
}