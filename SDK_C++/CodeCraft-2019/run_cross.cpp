#include "run_cross.h"
#include "read_file.h"
extern hash_map<int, Road> road_map;
extern hash_map<int, Cross> cross_map;

extern Car **car_sort_by_speed;
extern unsigned int time;

extern int lock ;

int all_car_end = 0;
void put_car(Car *car, Road *road, Cross *cross){
    static int car_head = 0;
    int next_road;
    // next_road = get_next_road(car_sort_by_speed[car_head]->start, car_sort_by_speed[car_head]->end, cross); //todo
    //放进车子（入队） todo
    car_sort_by_speed[car_head]->project->start_time = time;
    car_new_a_project_road(car_sort_by_speed[car_head], next_road);
    car_head++;
    
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

void run_all_cross(Cross *cross, int cross_num){
    int i;
    while(all_car_end == 0){
        for(i = 0; i < cross_num; i++){
            // run_a_cross(cross);
        }
        if(lock == 1){
           exit(0); 
        }
    }

}