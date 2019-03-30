#include "time.h"  //test
#include "project.h"
#include "read_file.h"

extern hash_map<int, Road *> road_map;
extern hash_map<int, Cross *> cross_map;
extern hash_map<int, int> cross_id_map;

extern int **time_precursor_matrix[MAX_SPEED];

extern CarList *carlist;
extern CarList *carlist_sroted;
extern int running_car_num;

extern unsigned int sys_time;

extern int all_car_end ;
extern int end_put_car ;

extern int lock ;

extern int map_capacity;
extern int surplus_map_capacity;
extern int ** cross_to_road;
extern int **projext_weight_matrix[MAX_SPEED];


void project_car(int car_num, int cross_num, int road_num, Car *car, Cross *cross, Road *road){
    // clock_t start, finish;  //test
    double duration;
    int i;

    int no_car = 0;
    sys_time =1;
    reset_all_pre_flow(road, road_num);
    init_time_precursor_matrix(cross, road, cross_num, road_num); //初始化放车的前驱矩阵
    init_projext_weight_matrix(cross, road, cross_num, road_num); 
    put_car(car, road, cross, cross_num, road_num);//第一次特殊，先加�
    sys_time ++;
    reset_all_car_to_ready(road, road_num);
    while(carlist != NULL){
     
        // start = clock(); 
        run_all_road(road, road_num);
        // finish = clock(); 
        // duration = (double)(finish-start) / CLOCKS_PER_SEC; 
        // printf("run_all_road:%lfs\n",duration);

        build_weight_matrix_by_capacity(cross, road, cross_num, road_num);
       

        // start = clock(); 
        project_all_waiting_car(road, road_num, cross, cross_num);
        // // finish = clock(); 
        // duration = (double)(finish-start) / CLOCKS_PER_SEC; 
        // printf("project_all_waiting_car:%lfs\n",duration);

        // start = clock(); 
        run_all_cross(cross, cross_num);
        // finish = clock(); 
        // duration = (double)(finish-start) / CLOCKS_PER_SEC; 
        // printf("run_all_cross:%lfs\n",duration);

        // start = clock();
        reset_all_pre_flow(road, road_num);
        // finish = clock(); 
        // duration = (double)(finish-start) / CLOCKS_PER_SEC; 
        // printf("reset_all_pre_flow:%lfs\n",duration);

        // start = clock();
        put_car(car, road, cross, cross_num, road_num);
        // finish = clock(); 
        // duration = (double)(finish-start) / CLOCKS_PER_SEC; 
        // printf("put_car:%lfs\n",duration);

        // start = clock();
        reset_all_car_to_ready(road, road_num);
        // finish = clock(); 
        // duration = (double)(finish-start) / CLOCKS_PER_SEC; 
        // printf("reset_all_car_to_ready:%lfs\n",duration);

 
        printf("ture_all_car : %d\n",all_car_running(road, road_num));
        // if(all_car_running(road, road_num)==0){
        //     printf("~");
        // }
        sys_time ++;
        // printf("\ntime = %d \n", sys_time);
    }

    reset_all_pre_flow(road, road_num);

    while(running_car_num != 0){
        run_all_road(road, road_num);
       
        build_weight_matrix_by_capacity(cross, road, cross_num, road_num);

        project_all_waiting_car(road, road_num, cross, cross_num);
        run_all_cross(cross, cross_num);
        reset_all_pre_flow(road, road_num);
        reset_all_car_to_ready(road, road_num);

        // printf("ture_all_car : %d\n",all_car_running(road, road_num));
        sys_time ++;
        // printf("\ntime = %d \n", sys_time);
    }

    // printf("\ntime = %d \n", sys_time);
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
                // if(que[i][j]->id==11046){            //test
                //     k = que[i][j]->id;
                // }
                if(que[i][j]->status == WAIT && j < get_min(que[i][j]->speed, this_road->limit)){
                    que[i][j]->next_step = get_next_road(this_road->cross_id_end, que[i][j]->end, all_road, cross, road_num, cross_num, que[i][j]->speed, this_road->cross_id_start,this_road->cross_id_end);
                    if(que[i][j]->next_step == -1){             //到达目的�
                        que[i][j]->next_dir = STRAIGHT;  //到达�  � 直行同优先级
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
                    break;                  //该条道路的后车就不用检�
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
                    // if(que[i][j]->id==11046){            //test
                    //     k = que[i][j]->id;
                    // }
                    if(que[i][j]->status == WAIT && j < get_min(que[i][j]->speed, this_road->limit)){
                        que[i][j]->next_step = get_next_road(this_road->cross_id_start, que[i][j]->end, all_road, cross, road_num, cross_num, que[i][j]->speed, this_road->cross_id_end, this_road->cross_id_start);
                        if(que[i][j]->next_step == -1){             //到达目的�
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
                        break;                  //该条道路的后车就不用检�
                    }
                }
            }
        }


    }
    
}


//可以按照书上进行优化   //最后的参数不是道路的起始和终止路口，而是需要看车子，车子开来的方向为起始~~~~
int get_next_road(int start, int end, Road *road, Cross *cross, int road_num, int cross_num, int speed, int this_road_start_id, int this_road_end_id){  
    int i, j, k = 0;
    
    int **weight_matrix = new_a_int_matrix(cross_num);
    for( i = 0; i < cross_num; i++)
    {
        for( j = 0; j < cross_num; j++)
        {
            weight_matrix[i][j] = projext_weight_matrix[speed][i][j];
        }
    }
    

    int *dist = (int *)malloc(sizeof(int)*cross_num);
    int *prev = (int *)malloc(sizeof(int)*cross_num);
    int *flag = (int *)malloc(sizeof(int)*cross_num);

    int src_id = cross_id_map[start];

    int shiedl_start = NIL; 
    int shiedl_end = NIL;
    if(this_road_start_id != NIL){
        shiedl_start = cross_id_map[this_road_start_id];
        shiedl_end = cross_id_map[this_road_end_id];

        weight_matrix[shiedl_end][shiedl_start] = NO_CONNECT;     //屏蔽的为当前�反向�
    }

    if(start == end){
        return -1;
    }
    //初始�
    for(i = 0; i < cross_num; i++){
        dist[i] = weight_matrix[src_id][i];
        prev[i] = NIL;
        flag[i] = UNSIGN;
    }  
    dist[src_id] = 0;
    flag[src_id] = SIGNED;
    //遍历除了start顶点的其他顶�
    for(i = 0; i < cross_num - 1; i++){
        //找到未标记的顶点的最短估计中最小�
        int min = NO_CONNECT;
        for(j = 0; j < cross_num; j++){
            if(flag[j] == UNSIGN && dist[j] < min){
                min = dist[j];
                k = j;
            }
        }
        flag[k] = SIGNED;
        //松弛
        for(j = 0; j < cross_num; j++){
            int tmp = (weight_matrix[k][j] == NO_CONNECT ? NO_CONNECT : (min + weight_matrix[k][j]));//防止溢出
            if(tmp == NO_CONNECT){
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
    int end_id = cross_id_map[end];
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

void init_time_precursor_matrix(Cross *cross, Road *road, int cross_num, int  road_num){
    int i;
    int **weight_matrix[MAX_SPEED];
    for(i = 1; i < MAX_SPEED; i++){
        weight_matrix[i] = build_weight_matrix_by_time(cross, road, cross_num, road_num, i);
        time_precursor_matrix[i] = get_precursor_matrix_floyd(weight_matrix[i], cross_num);
        free_a_matrix(weight_matrix[i], cross_num);
    }
}

void init_projext_weight_matrix(Cross *cross, Road *road, int cross_num, int  road_num){
    int i;
    for(i = 1; i < MAX_SPEED; i++){
        projext_weight_matrix[i] = new_a_int_matrix(cross_num);
    }
}



int get_put_road(int start, int end, int speed){
    int **precursor_matrix = time_precursor_matrix[speed];
    int src_id = cross_id_map[start];
    int end_id = cross_id_map[end];
    
    while(precursor_matrix[src_id][end_id] != src_id){
        end_id = precursor_matrix[src_id][end_id];
    }
    
    return cross_to_road[end_id][src_id];
}




int ** get_precursor_matrix_floyd(int **weight_matrix, int cross_num){          
    int ***iteration_matrix;
    int ***precursor_matrix; 
    int i, j, k, l, m;
    iteration_matrix = (int ***)malloc(sizeof(int**)*(cross_num+1));        //比结点多一个（�个）
    precursor_matrix = (int ***)malloc(sizeof(int**)*(cross_num+1));    
    for(k = 0; k < cross_num + 1; k++){
        iteration_matrix[k] = new_a_int_matrix(cross_num);  
        precursor_matrix[k] = new_a_int_matrix(cross_num);  
    }
    
    for(i = 0; i < cross_num; i++){
        for(j=0; j < cross_num; j++){
            (iteration_matrix[0])[i][j] = weight_matrix[i][j];
            if((i == j) || (weight_matrix[i][j] == NO_CONNECT)){
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
