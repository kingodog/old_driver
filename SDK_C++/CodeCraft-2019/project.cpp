#include "project.h"



void project_car(int car_num, int cross_num, int road_num, Car *car, Cross *cross, Road *road){
    // int matrix[5][5] = {{0, 3, 8, INFINITY_INT, -4},{INFINITY_INT, 0, INFINITY_INT, 1, 7},{INFINITY_INT, 4, 0, INFINITY_INT, INFINITY_INT},{2, INFINITY_INT, -5, 0, INFINITY_INT},{INFINITY_INT, INFINITY_INT, INFINITY_INT, 6, 0}};
    // int **matrix;
    // int **output_matrix;
    // int i, j;
    // int a1[5],a2[5],a3[5],a4[5],a5[5];

    // matrix = new_a_int_matrix(cross_num);
    // matrix = (int**)malloc(sizeof(int*)*5);  

    // int a1[5] = {0, 3, 8, INFINITY_INT, -4};
    // int a2[5] = {INFINITY_INT, 0, INFINITY_INT, 1, 7};
    // int a3[5] = {INFINITY_INT, 4, 0, INFINITY_INT, INFINITY_INT};
    // int a4[5] = {2, INFINITY_INT, -5, 0, INFINITY_INT};
    // int a5[5] = {INFINITY_INT, INFINITY_INT, INFINITY_INT, 6, 0};
    // matrix[0]=a1;
    // matrix[1]=a2;
    // matrix[2]=a3;
    // matrix[3]=a4;
    // matrix[4]=a5;
    // output_matrix = get_precursor_matrix_floyd(matrix, 5);
    // matrix = get_precursor_matrix_floyd(cross, road, cross_num, road_num);

    // for(i = 0; i < 5; i++){                                         //for test
    //     for(j = 0; j < 5; j++){
    //        printf("%d  ",output_matrix[i][j]);
    //     }
    //     printf("\n");
    // }

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
