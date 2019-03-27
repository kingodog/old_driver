#include "output_file.h"

void output_file(const char *path, Car *car, int car_num){
    FILE* fp = NULL;
    fp = fopen(path, "w+");    
    int i, j;
    for(i = 0; i < car_num; i++){
        fprintf(fp, "(%d, %d,", car[i].id, car[i].project->start_time);
        for(j = 0; j < car[i].project->road_num-1; j++)
        {
            fprintf(fp, " %d,", car[i].project->road[j]);
        }
        fprintf(fp, " %d)\n", car[i].project->road[j]);
    }
    fclose(fp);
    return;
}