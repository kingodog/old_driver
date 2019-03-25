#include "project.h"
#include "read_file.h"
extern unsigned int time;

extern hash_map<int, Road *> road_map;
extern hash_map<int, Cross *> cross_map;

extern CarList *carlist;
extern CarList *carlist_sroted;
extern int running_car_num;

extern unsigned int time;

extern int all_car_end ;
extern int end_put_car ;

extern int lock ;

extern int map_capacity;
extern int surplus_map_capacity;
extern int ** cross_to_road;

void project_car(int car_num, int cross_num, int road_num, Car *car, Cross *cross, Road *road){
    int no_car = 0;
    time =10;
    reset_all_pre_flow(road, road_num);
    put_car(car, road, cross, cross_num, road_num);//第一次特殊，先加车
    reset_all_car_to_ready(road, road_num);
    while(carlist != NULL){
        run_all_road(road, road_num);
        project_all_waiting_car(road, road_num, cross, cross_num);
        run_all_cross(cross, cross_num);
        reset_all_pre_flow(road, road_num);
        put_car(car, road, cross, cross_num, road_num);
        reset_all_car_to_ready(road, road_num);
        time ++;
    }

    reset_all_pre_flow(road, road_num);

    while(running_car_num != 0){
        run_all_road(road, road_num);
        project_all_waiting_car(road, road_num, cross, cross_num);
        run_all_cross(cross, cross_num);
        reset_all_pre_flow(road, road_num);
        reset_all_car_to_ready(road, road_num);
        time ++;
    }

    printf("\ntime = %d \n", time);
    return;
}



void project_all_waiting_car(Road *road, int road_num, Cross *cross, int cross_num){
    int i;
    for(i = 0; i < road_num; i++)
    {
        project_a_road_waiting_car(&road[i], road, road_num, cross, cross_num);
    }
}

void project_a_road_waiting_car(Road *this_road, Road *all_road, int road_num, Cross *cross, int cross_num){
    int i, j;
    int k;

    Car ***que = this_road->forward->lanes;
    for( i = 0; i < this_road->lanes_num; i++)              //正向
    {
        for( j = 0; j < this_road->length; j++)
        {
            if(que[i][j] !=NULL){
                if(que[i][j]->id==10018){            //test
                    k = que[i][j]->id;
                }
                if(que[i][j]->status == WAIT && i < get_min(que[i][j]->speed, this_road->limit)){
                    que[i][j]->next_step = get_next_road(this_road->cross_id_end, que[i][j]->end, all_road, cross, road_num, cross_num, que[i][j]->speed, this_road->cross_id_start,this_road->cross_id_end);
                    if(que[i][j]->next_step == -1){             //到达目的地
                        que[i][j]->next_dir = STRAIGHT;  //到达的   与  直行同优先级
                        this_road->pre_forward_surplus_flow += get_min(que[i][j]->speed, this_road->limit);
                        continue;
                    } else {
                        que[i][j]->next_dir = get_direction_by_road_id(*(cross_map[this_road->cross_id_end]), this_road->id, que[i][j]->next_step);
                        this_road->pre_forward_surplus_flow += get_min(que[i][j]->speed, this_road->limit);
                        if(road_map[que[i][j]->next_step]->cross_id_start == this_road->cross_id_end){
                            road_map[que[i][j]->next_step]->pre_forward_surplus_flow -= get_min(que[i][j]->speed, road_map[que[i][j]->next_step]->limit);
                        } else {
                            road_map[que[i][j]->next_step]->pre_back_surplus_flow -= get_min(que[i][j]->speed, road_map[que[i][j]->next_step]->limit);
                        }
                    }
                    
                } else {
                    break;                  //该条道路的后车就不用检测
                }
            }
        }
    }

    if(this_road->bothway == 1){                             //反向
        que = this_road->back->lanes;
        for( i = 0; i < this_road->lanes_num; i++)
        {
            for( j = 0; j < this_road->length; j++)
            {
                if(que[i][j] !=NULL){
                    if(que[i][j]->id==10018){            //test
                    k = que[i][j]->id;
                    }
                    if(que[i][j]->status == WAIT && i < get_min(que[i][j]->speed, this_road->limit)){
                        que[i][j]->next_step = get_next_road(this_road->cross_id_start, que[i][j]->end, all_road, cross, road_num, cross_num, que[i][j]->speed, this_road->cross_id_end, this_road->cross_id_start);
                        if(que[i][j]->next_step == -1){             //到达目的地
                            que[i][j]->next_dir = STRAIGHT;
                            this_road->pre_back_surplus_flow += get_min(que[i][j]->speed, this_road->limit);
                            continue;
                        } else {
                            que[i][j]->next_dir = get_direction_by_road_id(*cross_map[this_road->cross_id_start], this_road->id, que[i][j]->next_step);
                            this_road->pre_back_surplus_flow += get_min(que[i][j]->speed, this_road->limit);
                            if(road_map[que[i][j]->next_step]->cross_id_start == this_road->cross_id_start){
                                road_map[que[i][j]->next_step]->pre_back_surplus_flow -= get_min(que[i][j]->speed, road_map[que[i][j]->next_step]->limit);
                            } else {
                                road_map[que[i][j]->next_step]->pre_forward_surplus_flow -= get_min(que[i][j]->speed, road_map[que[i][j]->next_step]->limit);
                            }
                        }
                        
                    } else {
                        break;                  //该条道路的后车就不用检测
                    }
                }
            }
        }


    }
    
}


//可以按照书上进行优化   //最后的参数不是道路的起始和终止路口，而是需要看车子，车子开来的方向为起始~~~~
int get_next_road(int start, int end, Road *road, Cross *cross, int road_num, int cross_num, int speed, int this_road_start_id, int this_road_end_id){  
    int **weight_matrix = build_weight_matrix_by_capacity(cross, road, cross_num, road_num, speed);
    int *dist = (int *)malloc(sizeof(int)*cross_num);
    int *prev = (int *)malloc(sizeof(int)*cross_num);
    int *flag = (int *)malloc(sizeof(int)*cross_num);

    int i, j, k = 0;
    int src_id = ((int)(cross_map[start]) - (int)(cross))/sizeof(Cross);

    int shiedl_start = NIL; 
    int shiedl_end = NIL;
    if(this_road_start_id != NIL){
        shiedl_start = ((int)(cross_map[this_road_start_id]) - (int)(cross))/sizeof(Cross);
        shiedl_end = ((int)(cross_map[this_road_end_id]) - (int)(cross))/sizeof(Cross);

        weight_matrix[shiedl_end][shiedl_start] = NO_CONNECT;     //屏蔽的为当前的 反向到
    }

    

    if(start == end){
        return -1;
    }
    //初始化
    for(i = 0; i < cross_num; i++){
        dist[i] = weight_matrix[src_id][i];
        prev[i] = NIL;
        flag[i] = UNSIGN;
    }  
    dist[src_id] = 0;
    flag[src_id] = SIGNED;
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
    free_a_matrix(weight_matrix, cross_num);

    //找到end在cross数组中id
    int end_id = ((int)(cross_map[end]) - (int)(cross))/sizeof(Cross);
    while(prev[end_id] != NIL){
        end_id = prev[end_id];
    }

    free(dist);
    free(prev);
    free(flag);

    return cross_to_road[end_id][src_id];
    // Cross next_cross = cross[end_id];

    
    //TODO:通过下一个路口得到下一条路
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
