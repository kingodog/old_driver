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

hash_map<int, Road> road_map;
hash_map<int, Cross> cross_map;
unsigned int time = 0;
Car *car_head;
Car **car_sort_by_speed;

int main(int argc, char *argv[])
{
    std::cout << "Begin" << std::endl;
	
	// if(argc < 5){
	// 	std::cout << "please input args: carPath, roadPath, crossPath, answerPath" << std::endl;
	// 	exit(1);
	// }
	
	// std::string carPath(argv[1]);
	// std::string roadPath(argv[2]);
	// std::string crossPath(argv[3]);
	// std::string answerPath(argv[4]);
	
	// std::cout << "carPath is " << carPath << std::endl;
	// std::cout << "roadPath is " << roadPath << std::endl;
	// std::cout << "crossPath is " << crossPath << std::endl;
	// std::cout << "answerPath is " << answerPath << std::endl;

	int car_num;
    int road_num;
    int cross_num;
    Car *car=NULL;
    Cross *cross=NULL;
    Road *road=NULL;
	// CarProject *carproject;

	// carproject = (CarProject*)malloc(sizeof(CarProject)*2);			//for test
	// carproject[1].car_id=3001;
	// carproject[1].road_num=3;
	// carproject[1].start_time=1;
	// carproject[1].road=(int*)malloc(sizeof(int)*3);
	// carproject[1].road[0]=1001;
	// carproject[1].road[1]=1002;
	// carproject[1].road[2]=1003;

	// carproject[0].car_id=3000;
	// carproject[0].road_num=2;
	// carproject[0].start_time=2;
	// carproject[0].road=(int*)malloc(sizeof(int)*2);
	// carproject[0].road[0]=2001;
	// carproject[0].road[1]=2002;

    get_imformation(&car_num, &cross_num, &road_num, &car, &cross, &road, CAR_PATH,CROSS_PATH,ROAD_PATH);
	project_car(car_num, cross_num, road_num, car, cross, road);
	// output_file(OUTPUT_PATH, carproject,  2);						//for test
	// TODO:read input filebuf
	// TODO:process
	// TODO:write output file
	

	return 0;
}