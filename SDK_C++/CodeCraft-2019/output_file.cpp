#include "output_file.h"

void output_file(char *path, CarProject *carproject, int car_num){
    FILE* fp = NULL;
    fp = fopen(path, "w+");    
    int i, j;
    for(i = 0; i < car_num; i++){
        fprintf(fp, "(%d, %d,", carproject[i].car_id, carproject[i].start_time);
        for(j = 0; j < carproject[i].road_num-1; j++)
        {
            fprintf(fp, " %d,", carproject[i].road[j]);
        }
        fprintf(fp, " %d)\n", carproject[i].road[j]);
    }
    fclose(fp);
    return;
}