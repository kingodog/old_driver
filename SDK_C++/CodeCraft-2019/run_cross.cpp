#include "run_cross.h"
#include "read_file.h"

#define ALPHA    (float(1))

extern hash_map<int, Road> road_map;
extern hash_map<int, Cross> cross_map;

extern CarList *carlist;
extern CarList *carlist_sroted;
extern int running_car_num;

extern unsigned int time;

extern int all_car_end ;
extern int end_put_car ;

extern int lock ;

extern int map_capacity;
extern int surplus_map_capacity;


void put_car(Car *car, Road *road, Cross *cross){
    int next_step;
    CarList *p;
    p = carlist;
    RoadQue *que;
    int real_speed;
    int capacity_conversion = map_capacity * ALPHA;
    while(surplus_map_capacity <= capacity_conversion && p !=NULL){
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

        if(!que_is_full(que)){
            real_speed = get_min(p->car->speed, road_map[next_step].limit); 
            if(p->car->start == road_map[next_step].cross_id_start){            //同时跟新预先的流量和实际流量
                enqueue(&road_map[next_step], p->car, real_speed, FORWARD);
                road_map[next_step].forward_surplus_flow -= real_speed;
                road_map[next_step].pre_forward_surplus_flow = road_map[next_step].forward_surplus_flow;
            } else {
                enqueue(&road_map[next_step], p->car, real_speed, BACK);
                road_map[next_step].back_surplus_flow -= real_speed;
                road_map[next_step].pre_back_surplus_flow = road_map[next_step].back_surplus_flow;
            }
            p->car->project->start_time = time;
            p->car->status = END;
            car_new_a_project_road(p->car, next_step);
            p->car->next_step = -1;
            p->car->next_dir = -1;
            delete_car_from_list(&p);                                           //函数包括了p=p->next;
            surplus_map_capacity -= real_speed;
            running_car_num ++;
        } else {
            p = p->next;
        }

    }
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

void run_all_cross(Cross *cross, int cross_num){    //todo
    int i;
    while(all_car_end == 0){
        all_car_end = 1;
        lock = 1;
        for(i = 0; i < cross_num; i++){
            run_a_cross(&(cross[i]));
        }
        if(lock == 1){
            printf("\n***************************lock***************************!\n");
            exit(0); 
        }
    }
}



void run_a_cross(Cross *cross){  
    int end_flag = 0;
    int i;
    RoadQue *road[4];
    for(i = 0; i < cross->total_road; i++){                     //可优化
        if(road_map[cross->road_id_sorted[i]].cross_id_start == cross->id){
            road[i] = road_map[cross->road_id_sorted[i]].back;
        } else {
            road[i] = road_map[cross->road_id_sorted[i]].forward;
        }
    }
    while(end_flag == 0){
        end_flag = 1;
        for(i = 0; i < cross->total_road; i++){
            run_a_road(cross, road[i], &(road_map[cross->road_id_sorted[i]]), &end_flag);          //可优化（不用地图）
        }
    }    
}

void run_a_road(Cross *cross, RoadQue *way, Road *road, int *end_flag){          //可大优化
    int i, j;
    Car *car_p;
    Car *forward_car = NULL;
    int speed;
    int distance;
    int n, m;                  
    int next_real_speed;
    int *curr_flow;
    int *next_flow;
    if(que_is_empty(way)){   
        return ;
    }
    
    if (road->forward == way) {
        curr_flow = &(road->forward_surplus_flow);
    } else if(road->back == way){
        curr_flow = &(road->back_surplus_flow);
    } else {
        printf("run_a_road_is_wrong!\n");
    }
    

    for(i = 0; i < 4; i++){
        if(cross->road_id[i] == road->id){
            n = i;                          //用于寻找其它三个方向的道
            break;
        }
    }

    for(j = 0; j < road->length; j++){
        for(i = 0; i < road->lanes_num; i++){
            switch (way->lanes[i][j]->status)
            {
                case WAIT:
                    for(distance = 1; distance <= j; distance++){
                        if(way->lanes[i][j - distance] != NULL){
                            forward_car = way->lanes[i][j];
                            break;
                        }
                    }
                
                    if( forward_car == NULL){                   //无遮挡
                        distance = j;
                        speed = get_min(way->lanes[i][j]->speed, road->limit);
                        if(speed <= distance){                      //不过路口   可能不需要  暂时保留
                            way->lanes[i][j+speed] = way->lanes[i][j];
                            way->lanes[i][j] = NULL;
                            way->lanes[i][j+speed]->status = END;
                            way->lanes[i][0]->next_dir = -1;
                            way->lanes[i][0]->next_step = -1;
                            lock = 0;
                            *end_flag = 0;                                  //可能可优化
                            adjust_a_lane(j, road->length, way->lanes[i], road->limit);
                        } else if (way->lanes[i][j]->next_dir == -1){            //此路口为终点
                            way->lanes[i][j]->status = END;
                            way->lanes[i][j] = NULL;
                            running_car_num --;
                            surplus_map_capacity += speed;
                            lock = 0;
                            *end_flag = 0;                      //可能可优化
                            *curr_flow = *curr_flow + speed;
                            adjust_a_lane(j, road->length, way->lanes[i], road->limit);
                        } else {
                            next_real_speed = get_min(road_map[way->lanes[i][j]->next_step].limit, way->lanes[i][j]->speed);
                            if(way->lanes[i][j]->next_dir == STRAIGHT){
                                m = (n+2)%4;
                            }

                            if(way->lanes[i][j]->next_dir == LEFT){
                                car_p = get_right_road_first_car(cross, road->id);
                                if(car_p !=NULL){       
                                    if(car_p->next_dir == STRAIGHT){   //有车子confict
                                        // *end_flag = 0;
                                        return ; 
                                    }
                                }
                                m = (n+1)%4; 
                            }

                            if(way->lanes[i][j]->next_dir == RIGHT){
                                car_p = get_left_road_first_car(cross, road->id);
                                if(car_p !=NULL){       
                                    if(car_p->next_dir == STRAIGHT){ 
                                        // *end_flag = 0;
                                        return ; 
                                    }
                                } 
                                car_p = get_across_road_first_car(cross, road->id);
                                if(car_p !=NULL){       
                                    if(car_p->next_dir == LEFT){ 
                                        // *end_flag = 0;
                                        return ; 
                                    }
                                }
                                m = (n+3)%4;  
                            }
                            
                            if((next_real_speed - j) <= 0 ){        //走到头终止
                                way->lanes[i][0] = way->lanes[i][j];
                                
                                way->lanes[i][0]->status = END;
                                way->lanes[i][0]->next_dir = -1;
                                way->lanes[i][0]->next_step = -1;
                                *end_flag = 0;
                                lock = 0;
                                way->lanes[i][j] = NULL;
                                adjust_a_lane(j, road->length, way->lanes[i], road->limit);
                                break; 
                            } else {
                                switch(cross_through(cross->road[m], way->lanes[i][j], next_real_speed, cross->road_dir_out[m])){
                                    case COME_ON:
                                        way->lanes[i][j]->status = END;
                                        car_new_a_project_road(way->lanes[i][j], way->lanes[i][0]->next_step);
                                        way->lanes[i][j]->next_dir = -1;
                                        way->lanes[i][j]->next_step = -1;
                                        *end_flag = 0;
                                        lock = 0;
                                        if(road_map[way->lanes[i][j]->next_step].cross_id_start == cross->id){
                                            road_map[way->lanes[i][j]->next_step].forward_surplus_flow -= next_real_speed;
                                        } else {
                                            road_map[way->lanes[i][j]->next_step].back_surplus_flow -= next_real_speed;
                                        }
                                        *curr_flow = *curr_flow + speed;
                                        way->lanes[i][j] = NULL;
                                        adjust_a_lane(j, road->length, way->lanes[i], road->limit);
                                    break;
                                    case PLEASE_WAIT:               //本条路结束 遍历
                                        all_car_end = 0;
                                    return;
                                    case GET_OUT:
                                        lock = 0;
                                        way->lanes[i][0] = way->lanes[i][j];
                                        way->lanes[i][j] = NULL;
                                        way->lanes[i][0]->status = END;
                                        way->lanes[i][0]->next_dir = -1;
                                        way->lanes[i][0]->next_step = -1;
                                        *end_flag = 0;
                                        adjust_a_lane(j, road->length, way->lanes[i], road->limit);
                                    break;
                                }
                            } 
                        }
                    } else {
                        printf("project wrong!\n");
                    } 
                    forward_car =NULL;
                    break;
                case END:
                                                    //可优化
                    break;
                default:
                    break;
            }
        }
    }
    return ;
}

void adjust_a_lane(int start, int end, Car **lane, int limit_speed){            //todo   可能出错
    int i;
    int distance;
    int real_speed;
    int move_distance;
    Car *car;
    Car *pre_car = NULL;
    for(i = start; i < end; i++)                
    {
        if(lane[i] !=NULL){
            if(lane[i]->status ==END){
                return ;
            }
            real_speed = get_min(lane[i]->speed, limit_speed);
            for(distance = 1; distance <= i; distance++){             //可优化 保存之前的
                if(lane[i - distance] != NULL){
                    pre_car = lane[i - distance];
                    break;
                }
            }                               //前车状态一定为END，   可考虑用来测试代码
            
            distance --; 
            if(pre_car == NULL){
                if(real_speed <= distance){
                    lane[i - real_speed] = lane[i];
                    lane[i] = NULL;
                    lane[i - real_speed]->status = END;
                    continue;
                } else {                //如果前面距离不够，直接溜
                    return;
                }
            } else {
                move_distance = get_min(real_speed, distance);
                lane[i - move_distance] = lane[i];
                lane[i] = NULL;
                lane[i - move_distance]->status = END;
                continue;
            }
        }
    }
}

// int get_first_lane_tail_distance(int length, RoadQue *lane){
//     int i;
//     for(i = length-1; i >= 0; i--){
//         if(lane->lanes[0][i] !=NULL){
//             return (length - i -1);
//         }
//     }
//     return length;
// }


Car *get_left_road_first_car(Cross * corss, int self_road_id){
    int i;
    RoadQue *left_input;
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
    RoadQue *right_input;
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
    RoadQue *across_input;
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




