#include "iostream"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include "read_file.h"
#include "project.h"
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
    get_imformation(&car_num, &cross_num, &road_num, &car, &cross, &road, CAR_PATH,CROSS_PATH,ROAD_PATH);
	project_car(car_num, cross_num, road_num, car, cross, road);
	// TODO:read input filebuf
	// TODO:process
	// TODO:write output file
	
	return 0;
}