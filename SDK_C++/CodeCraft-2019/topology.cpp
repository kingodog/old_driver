#include "topology.h"

float ** build_weight_matrix(Cross *cross, Road *road, int cross_num, int  road_num,int speed){
    float **weight_matrix;
    int i, j;
    weight_matrix = (float**)malloc(sizeof(float*)*cross_num);  
    for(i = 0; i < cross_num; i++){
        weight_matrix[i] = (float*)malloc(sizeof(float)*cross_num); 
    }

    for(i = 0; i < cross_num; i++){
        for(j = 0; j < cross_num; j++){
            if(i == j){
                weight_matrix[i][j] = 0;
            } else {
                weight_matrix[i][j] = get_road_weight(cross[i].id, cross[j].id, road, road_num, speed);
            }
        }
    }
    return weight_matrix;
}

float get_road_weight(int start_id, int end_id, Road *road, int road_num, int speed){             //根据速度来获取某两个点的权值
    int i;
    for(i = 0; i < road_num; i++){
        if(road[i].cross_id_strat == start_id && road[i].cross_id_end == end_id){
            return (float)1 / ceil(float(road[i].lenth) / get_min(speed, road[i].limit));                                    
        }   else if (road[i].cross_id_strat == end_id && road[i].cross_id_end == start_id && road[i].ways == 1){
            return (float)1 / ceil(float(road[i].lenth) / get_min(speed, road[i].limit)); 
        }
    }
    return NO_CONNECT;
}

int get_min(int a, int b){
    if( a > b ){
        return b;
    } else {
        return a;
    }
}