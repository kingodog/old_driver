#include "topology.h"

int ** build_weight_matrix(Cross *cross, Road *road, int cross_num, int  road_num,int speed){
    int **weight_matrix;
    int i, j;
    weight_matrix = (int**)malloc(sizeof(int*)*cross_num);  
    for(i = 0; i < cross_num; i++){
        weight_matrix[i] = (int*)malloc(sizeof(int)*cross_num); 
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

int get_road_weight(int start_id, int end_id, Road *road, int road_num, int speed){             //根据速度来获取某两个点的权值
    int i;
    for(i = 0; i < road_num; i++){
        if(road[i].cross_id_strat == start_id && road[i].cross_id_end == end_id){
            return ceil(float(road[i].lenth) / get_min(speed, road[i].limit));                                    
        }   else if (road[i].cross_id_strat == end_id && road[i].cross_id_end == start_id && road[i].ways == 1){
            return ceil(float(road[i].lenth) / get_min(speed, road[i].limit)); 
        }
    }
    return NO_CONNECT;
}

int ** build_capacity_matrix_by_ways(Cross *cross, Road *road, int cross_num, int  road_num){
    int **capacity_matrix;
    int i, j;
    capacity_matrix = (int**)malloc(sizeof(int*)*cross_num);  
    for(i = 0; i < cross_num; i++){
        capacity_matrix[i] = (int*)malloc(sizeof(int)*cross_num); 
    }

    for(i = 0; i < cross_num; i++){
        for(j = 0; j < cross_num; j++){
            if(i == j){
                capacity_matrix[i][j] = 0;
            } else {
                capacity_matrix[i][j] = get_road_capacity_by_ways(cross[i].id, cross[j].id, road, road_num);
            }
        }
    }
    return capacity_matrix;
}

int get_road_capacity_by_ways(int start_id, int end_id, Road *road, int road_num){             //根据道路宽度确定容量
    int i;
    for(i = 0; i < road_num; i++){
        if(road[i].cross_id_strat == start_id && road[i].cross_id_end == end_id){
            return (road[i].ways);                                    
        } else if (road[i].cross_id_strat == end_id && road[i].cross_id_end == start_id && road[i].ways == 1){
            return (road[i].ways);   
        }
    }
    return NO_CONNECT;
}


int ** build_cost_matrix_by_length(Cross *cross, Road *road, int cross_num, int  road_num){
    int **cost_matrix;
    int i, j;
    cost_matrix = (int**)malloc(sizeof(int*)*cross_num);  
    for(i = 0; i < cross_num; i++){
        cost_matrix[i] = (int*)malloc(sizeof(int)*cross_num); 
    }

    for(i = 0; i < cross_num; i++){
        for(j = 0; j < cross_num; j++){
            if(i == j){
                cost_matrix[i][j] = 0;
            } else {
                cost_matrix[i][j] = get_road_cost_by_length(cross[i].id, cross[j].id, road, road_num);
            }
        }
    }
    return cost_matrix;
}

int get_road_cost_by_length(int start_id, int end_id, Road *road, int road_num){             //根据道路长度确定花费
    int i;
    for(i = 0; i < road_num; i++){
        if(road[i].cross_id_strat == start_id && road[i].cross_id_end == end_id){
            return (road[i].lenth);                                    
        } else if (road[i].cross_id_strat == end_id && road[i].cross_id_end == start_id && road[i].ways == 1){
            return (road[i].lenth);   
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