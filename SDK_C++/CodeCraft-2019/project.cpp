#include "project.h"



void project_car(int car_num, int cross_num, int road_num, Car *car, Cross *cross, Road *road){
    int **matrix;
    int i, j;

    matrix = build_capacity_matrix_by_ways(cross, road, cross_num, road_num);

    for(i = 0; i < cross_num; i++){                                         //for test
        for(j = 0; j < cross_num; j++){
           printf("%d  ",matrix[i][j]);
        }
        printf("\n");
    }


    return;
}