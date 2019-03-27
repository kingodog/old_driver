#include "iostream"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include "read_file.h"
#include "project.h"
#include "topology.h"
#include "output_file.h"
#include "run_road.h"

hash_map<int, Road *> road_map;
hash_map<int, Cross *> cross_map;
hash_map<int, int > cross_id_map;
unsigned int sys_time = 0;
CarList *carlist;
CarList *carlist_sroted;
int lock = 1;
int running_car_num = 0;
int map_capacity = 0;
int surplus_map_capacity = 0;

int all_car_end = 0;
int end_put_car = 0;

int ** cross_to_road;

int main(int argc, char *argv[])
{
    std::cout << "Begin" << std::endl;
	
	if(argc < 5){
		std::cout << "please input args: carPath, roadPath, crossPath, answerPath" << std::endl;
		exit(1);
	}
	
	std::string carPath(argv[1]);
	std::string roadPath(argv[2]);
	std::string crossPath(argv[3]);
	std::string answerPath(argv[4]);
	
	std::cout << "carPath is " << carPath << std::endl;
	std::cout << "roadPath is " << roadPath << std::endl;
	std::cout << "crossPath is " << crossPath << std::endl;
	std::cout << "answerPath is " << answerPath << std::endl;

	int car_num;
    int road_num;
    int cross_num;
    Car *car=NULL;
    Cross *cross=NULL;
    Road *road=NULL;

    get_imformation(&car_num, &cross_num, &road_num, &car, &cross, &road, carPath.c_str(), crossPath.c_str(), roadPath.c_str());
	project_car(car_num, cross_num, road_num, car, cross, road);
	output_file(answerPath.c_str(), car,  car_num);				

	return 0;
}