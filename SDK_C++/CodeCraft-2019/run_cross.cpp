#include "run_cross.h"
#include "read_file.h"

#define ALPHA    (float(1))

extern hash_map<int, Road> road_map;
extern hash_map<int, Cross> cross_map;

extern CarList *carlist;
extern unsigned int time;

extern int lock ;
extern int running_car_num;

extern int all_car_end ;
extern int end_put_car ;

extern int map_capacity;
extern int surplus_map_capacity;

void put_car(Car *car, Road *road, Cross *cross){
    int next_step;
    CarList *p;
    p = carlist;
    Road_que *que;
    int capacity_conversion = map_capacity * ALPHA;
    while(surplus_map_capacity <= capacity_conversion || P !=NULL){
        // next_step = get_next_road(p->car->start, p->car->end, road, cross, p->car->speed);
        if(next_step == -1){
             p = p->next;
             continue;
        }

        if(p->car->start == road_map[next_step].cross_id_start){
            que = road_map[next_step].forward;
        } else {
            que = road_map[next_step].back;
        }

        if(!is_full(que)){
            //放入车   //todo  //路的容量
            
            p->car->project->start_time = time;
            p->car->status = END;
            car_new_a_project_road(p->car, next_step);
            p->car->next_step = -1;
            p->car->next_dir = -1;
            delete_car_from_list(&p);   //函数包括了p=p->next;
            map_capacity = map_capacity - MIN(p->car->speed, road_map[next_step].limit);
            running_car_num ++;
        } else {
            p = p->next;
        }

    }

    time++;
    
}

int get_direction_by_road_id(Cross cross, int start_id, int end_id){
    int i;
    int start, end;
    for(i = 0; i < 4; i++){
        if(cross.road_id[i] == start_id){
			start = i;
        }
		if(cross.road_id[i] == end_id){
			end = i;
        }
    }
    if(start - end == 2 || start - end == -2){
		return STRAIGHT;
	} else if(start - end == 3 || start - end == -1){
		return LEFT;
	} else {
		return RIGHT;
	}
}

void car_new_a_project_road(Car *car, int road_id){
    car->project->road[car->project->road_num] = road_id;
    car->project->road_num++;
    if(car->project->road_num >= CAR_MAX_RECORD){
        printf("out of car road memery!\n");
    }
}
    project_cross_waiting_car(&(cross[i]);
void run_all_cross(Cross *cross, int cross_num){
    int i;
    while(all_car_end == 0){
        all_car_end = 1;
        for(i = 0; i < cross_num; i++){
            run_a_cross(&(cross[i]);
        }
        if(lock == 1){
           exit(0); 
        }
    }

}

void project_cross_waiting_car(Cross *cross){
    // int i,j,k;
    // Road_que *road[4];
    // for(i = 0; i < cross->total_road; i++){
    //     if(road_map[cross->road_id_sorted[i]].cross_id_start == cross->id){
    //         road[i] = road_map[cross->road_id_sorted[i]].back;
    //     } else {
    //         road[i] = road_map[cross->road_id_sorted[i]].forward;
    //     }
    // }
    // for(i = 0; i < cross->total_road; i++){
    //     for(j = 0; j < road_map[cross->road_id_sorted[i]].lanes_num; j++){
    //         for(k = 0; k < road_map[cross->road_id_sorted[i]].length; k++){
    //             if()
    //         }
    //     }
    // }
}

void run_a_cross(Cross *cross){
    int end_flag = 0;
    int i;
    Road_que *road[4];
    for(i = 0; i < cross->total_road; i++){
        if(road_map[cross->road_id_sorted[i]].cross_id_start == cross->id){
            road[i] = road_map[cross->road_id_sorted[i]].back;
        } else {
            road[i] = road_map[cross->road_id_sorted[i]].forward;
        }
    }
    while(end_flag == 0){
        end_flag = 1;
        for(i = 0; i < cross->total_road; i++){
            run_a_road();

        }
    }    
}

int run_a_road(Cross *cross, Road_que *way, Road *road){
    int i, j;
    Car *car_p;
    Car *forward_car = NULL;
    int speed;
    int distance;
    if(way->head[0] < 0){   //todo 改为是否为空的函数
        return -1;
    }

    for(i = 0; i < road->length; i++){
        for(j = 0; j < road->lanes_num){
            switch (way->lanes[i][j]->status)
            {
                case WAIT:
                    for(distance = 1; distance <= i; distance++){
                        if(way->lanes[i - distance][j] != NULL){
                            forward_car = way->lanes[i][j];
                            break;
                        }
                    }
                    if( forward_car == NULL){                   //无遮挡
                        distance = i;
                        speed = get_min(way->lanes[i][j]->speed, road->limit);
                        if(speed <= distance){                      //不过路口
                            //前进
                            //todo
                            way->lanes[i][j]->status = END;
                        } else if (way->lanes[i][j]->next_dir == -1){            //此路口为终点
                            way->lanes[i][j]->status = END;
                            way->lanes[i][j] = NULL;
                            running_car_num --;
                            map_capacity = map_capacity + MIN(way->lanes[i][j]->speed, road.limit);
                            //... //todo
                        } else if(way->lanes[i][j]->next_dir == STRAIGHT){
                            // //放入车(way->lanes[i][j]->next_step)   //todo
                            //..
                            //调整当前道的后车  
                        } else if(way->lanes[i][j]->next_dir == LEFT){
                            car_p = get_right_road_first_car(cross, road->id);
                            if(car_p !=NULL){       
                                if(car_p->next_dir == STRAIGHT){   //有车子confict
                                    return ; 
                                }
                            } 
                            // //放入车(way->lanes[i][j]->next_step)   //todo    过路口
                            //..
                            //调整当前道的后车                                
                        } else if(way->lanes[i][j]->next_dir == RIGHT){
                            car_p = get_left_road_first_car(cross, road->id);
                            if(car_p !=NULL){       
                                if(car_p->next_dir == STRAIGHT){ 
                                    return ; 
                                }
                            } 
                            car_p = get_left_across_first_car(cross, road->id);
                            if(car_p !=NULL){       
                                if(car_p->next_dir == LEFT){ 
                                    return ; 
                                }
                            } 

                            // //放入车(way->lanes[i][j]->next_step)   //todo    过路口
                            //..
                            //调整当前道的后车                                  
                        }
                    } else {
                        printf("project wrong!\n");
                    } 
                    forward_car =NULL;
                    break;
                case END:
                    //待优化时使用
                    break;
                default:
                    break;
            }
        }
    }


}

Car *get_left_road_first_car(Cross * corss, int self_road_id){
    int i;
    Road_que *left_input;
    for( i = 0; i < 4; i++)
    {
        if(corss->road_id[i] = self_road_id){
            break;
        }
    }

    left_input = corss->road_dir[(i+1)%4];

    if(left_input == NULL){
        return NULL;
    } else if( left_input->head[0] < 0){
        return NULL;
    } else {
        return (left_input->lanes[left_input->head[0]][left_input->head[1]]);
    }
}

Car *get_right_road_first_car(Cross * corss, int self_road_id){
    int i;
    Road_que *right_input;
    for( i = 0; i < 4; i++)
    {
        if(corss->road_id[i] = self_road_id){
            break;
        }
    }

    right_input = corss->road_dir[(i+3)%4];

    if(right_input == NULL){
        return NULL;
    } else if( right_input->head[0] < 0){
        return NULL;
    } else {
        return (right_input->lanes[right_input->head[0]][right_input->head[1]]);
    }
}

Car *get_across_road_first_car(Cross * corss, int self_road_id){
    int i;
    Road_que *across_input;
    for( i = 0; i < 4; i++)
    {
        if(corss->road_id[i] = self_road_id){
            break;
        }
    }

    across_input = corss->road_dir[(i+2)%4];

    if(across_input == NULL){
        return NULL;
    } else if( across_input->head[0] < 0){
        return NULL;
    } else {
        return (across_input->lanes[across_input->head[0]][across_input->head[1]]);
    }
}




