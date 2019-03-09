#include "read_file.h"

void get_imformation(int *car_num, int *cross_num, int *road_num, Car **car, Cross **cross, Road **road){
    get_car_imformation(CAR_PATH,car_num, car);
    get_cross_imformation(CROSS_PATH,cross_num, cross);
    get_road_imformation(ROAD_PATH,road_num, road);
}

void get_car_imformation(char *path, int *car_num, Car **car){
    FILE *fp1 = fopen(path,"r");
    char StrLine[1024];
    char *str;
    int i=0;
    if(!fp1)
    {
        printf("can not open the file!\n");
    }
    while(!feof(fp1)){
        fgets(StrLine,1024,fp1);
        i++;
    }

    *car_num=(i+1)/2;                 //包含无效行
    (*car)=(Car *)malloc(sizeof(Car)*(*car_num));
    i=0;
    fseek(fp1, 0, 0);

    while(!feof(fp1)){
        fgets(StrLine,1024,fp1);
        if(StrLine[0] != '('){
            continue;
        }
        str = StrLine;
        get_next_int(&str, &((*car)[i].id));
        get_next_int(&str, &((*car)[i].start));
        get_next_int(&str, &((*car)[i].end));
        get_next_int(&str, &((*car)[i].speed));
        get_next_int(&str, &((*car)[i].start_time));
        i++;
    }
    fclose(fp1); 
}

void get_cross_imformation(char *path,int *cross_num, Cross **cross){
    FILE *fp1 = fopen(path,"r");
    char StrLine[1024];
    char *str;
    int i=0;
    if(!fp1)
    {
        printf("can not open the file!\n");
    }
    while(!feof(fp1)){
        fgets(StrLine,1024,fp1);
        i++;
    }

    *cross_num=(i+1)/2;                 //包含无效行
    (*cross)=(Cross *)malloc(sizeof(Cross)*(*cross_num));
    i=0;
    fseek(fp1, 0, 0);

    while(!feof(fp1)){
        fgets(StrLine,1024,fp1);
        if(StrLine[0] != '('){
            continue;
        }
        str = StrLine;
        get_next_int(&str, &((*cross)[i].id));
        get_next_int(&str, &((*cross)[i].road_id[0]));
        get_next_int(&str, &((*cross)[i].road_id[1]));
        get_next_int(&str, &((*cross)[i].road_id[2]));
        get_next_int(&str, &((*cross)[i].road_id[3]));
        i++;
    }
    fclose(fp1);
}

void get_road_imformation(char *path,int *road_num, Road **road){
    FILE *fp1 = fopen(path,"r");
    char StrLine[1024];
    char *str;
    char *data;
    int i=0;
    if(!fp1)
    {
        printf("can not open the file!\n");
    }
    while(!feof(fp1)){
        fgets(StrLine,1024,fp1);
        i++;
    }

    *road_num=(i+1)/2;                 //包含无效行
    (*road)=(Road *)malloc(sizeof(Road)*(*road_num));
    i=0;
    fseek(fp1, 0, 0);

    while(!feof(fp1)){
        fgets(StrLine,1024,fp1);
        if(StrLine[0] != '('){
            continue;
        }
        str = StrLine;
        get_next_int(&str, &((*road)[i].id));
        get_next_int(&str, &((*road)[i].lenth));
        get_next_int(&str, &((*road)[i].limit));
        get_next_int(&str, &((*road)[i].ways));
        get_next_int(&str, &((*road)[i].cross_id_strat));
        get_next_int(&str, &((*road)[i].cross_id_end));
        get_next_int(&str, &((*road)[i].bothway));
        i++;
    }
    fclose(fp1);
}

int get_next_int(char **str, int *num){
    int i=0,j=0,k=0;
    char *data; 
    char str_num[1024];
    char a = data[j];
    data =  *str;                                              //指向 字符串
    while(((data[j] < '0')||(data[j] > '9'))&&(data[j] != '-')){
        if(data[j] == '\0'){            //无效行
            return DEFEATED;
        }
        j++;
    }
    i=j;
    while(((data[i] >= '0')&&(data[i] <= '9'))||(data[i] == '-')){
        i++;
    }
    for(k=0;k<i-j;k++){
        str_num[k] = data[j+k];
    }
    str_num[k] = '\0';
    *num = atoi(str_num);
    *str = &data[i];            //改变指针位置，为下一次读取准备
    return SUCCESSFUL;
}
