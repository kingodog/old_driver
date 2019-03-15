#include "project.h"



void project_car(int car_num, int cross_num, int road_num, Car *car, Cross *cross, Road *road){
    float **weight_matrix;
    int i, j;

    weight_matrix = build_weight_matrix(cross, road, cross_num, road_num, 2);

    // for(i = 0; i < cross_num; i++){
    //     for(j = 0; j < cross_num; j++){
    //        printf("%0.1f  ",weight_matrix[i][j]);
    //     }
    //     printf("\n");
    // }
    // return;
}