#include "read_file.h"

extern CarList *carlist;
int car_classified[11];

extern hash_map<int, Road> road_map;
extern hash_map<int, Cross> cross_map;

void get_imformation(int *car_num, int *cross_num, int *road_num, 
        Car **car, Cross **cross, Road **road, 
        char *car_path, char *cross_path, char *road_path){

    get_road_imformation(road_path, road_num, road);            //不可改变先后顺序
    get_cross_imformation(cross_path,cross_num, cross);
    get_car_imformation(car_path,car_num, car, road_num);
    sort_car_by_speed(*car, *car_num);
}

void get_car_imformation(char *path, int *car_num, Car **car, int *road_num){

    FILE *fp1 = fopen(path,"r");
    char StrLine[1024];
    char *str;
    int i=0;
    for(i = 0; i < 10; i++){     //初始化数组;
        car_classified[i] = 0;
    }
    
    if(!fp1)
    {
        printf("can not open the file!\n");
        return;
    }
    i = 0;
    while(!feof(fp1)){
        fgets(StrLine,1024,fp1);
         if(StrLine[0] == '('){
            i++;
        }
    }

    *car_num = i;                 //包含无效行
    (*car) = (Car *)malloc(sizeof(Car)*(*car_num));
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
        (*car)[i].status = UNBORN;
        (*car)[i].project = (CarProject *)malloc(sizeof(CarProject));
        (*car)[i].project->road_num = 0;
        car_classified[(*car)[i].speed]++;
        i++;
    }
    fclose(fp1); 
}


void get_cross_imformation(char *path,int *cross_num, Cross **cross){
    FILE *fp1 = fopen(path,"r");
    char StrLine[1024];
    char *str;
    int i=0;
    int k=0;
    if(!fp1)
    {
        printf("can not open the file!\n");
        return;
    }
    while(!feof(fp1)){
        fgets(StrLine,1024,fp1);
         if(StrLine[0] == '('){
            i++;
        }
    }

    *cross_num = i;                 //包含无效行
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
        sort_cross_road_id(&((*cross)[i]));
        for(k = 0;k < 4;k++){
            if((*cross)[i].road_id[k] == -1){
                (*cross)[i].road_dir[k] == NULL;
                (*cross)[i].road_dir_out[k] == NULL;
                (*cross)[i].road[k] = NULL;
            } else {
                (*cross)[i].road[k] = &(road_map[(*cross)[i].road_id[k]]);
                if(road_map[(*cross)[i].road_id[k]].cross_id_start == (*cross)[i].id){   //特别注意，
                    if(road_map[(*cross)[i].road_id[k]].bothway == 1){
                        (*cross)[i].road_dir[k] == road_map[(*cross)[i].road_id[k]].back;
                        (*cross)[i].road_dir_out[k] == road_map[(*cross)[i].road_id[k]].forward;
                    } else {
                        (*cross)[i].road_dir[k] == NULL;
                        (*cross)[i].road_dir_out[k] == road_map[(*cross)[i].road_id[k]].forward;
                    }
                } else {
                    if(road_map[(*cross)[i].road_id[k]].bothway == 1){
                        (*cross)[i].road_dir[k] == road_map[(*cross)[i].road_id[k]].forward;
                        (*cross)[i].road_dir_out[k] == road_map[(*cross)[i].road_id[k]].back;
                    } else {
                        (*cross)[i].road_dir[k] == road_map[(*cross)[i].road_id[k]].forward;
                        (*cross)[i].road_dir_out[k] == NULL;

                    }
                }
            }
        }
        
        cross_map[(*cross)[i].id] = (*cross)[i];

        i++;
    }
    fclose(fp1);
}

void  sort_cross_road_id(Cross *cross){
    int i,j;
    int total_road = 0;
    j = 0;
    for(i = 0; i < 4; i++){
        cross->road_id_sorted[i] = -1;
        if(cross->road_id[i] != -1){
            cross->road_id_sorted[j] = cross->road_id[i];
            total_road ++;
            j++;
        }
    }
    cross->total_road = total_road;
    switch (total_road)
    {
        case 1:
            break;
        case 2:
            if(cross->road_id_sorted[0] > cross->road_id_sorted[1]){
                exchange_int_num(&(cross->road_id_sorted[0]), &(cross->road_id_sorted[1]));
            }
            break;
        case 3:
            for(i = 0;i < 2; i++){
                for(j = 2;j > i; j--){
                    if(cross->road_id_sorted[j-1] > cross->road_id_sorted[j]){
                        exchange_int_num(&(cross->road_id_sorted[j-1]), &(cross->road_id_sorted[j]));
                    }
                }
            }
            break;
        case 4:
            for(i = 0;i < 3; i++){
                for(j = 3;j > i; j--){
                    if(cross->road_id_sorted[j-1] > cross->road_id_sorted[j]){
                        exchange_int_num(&(cross->road_id_sorted[j-1]), &(cross->road_id_sorted[j]));
                    }
                }
            }
            break;
        default:
            printf("sort_cross_road_id is wrong!\n");
            break;
    }
}

void exchange_int_num(int *a, int *b){
    int k;
    k = *a;
    *a = *b;
    *b = k;
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
        return;
    }
    while(!feof(fp1)){
        fgets(StrLine,1024,fp1);
         if(StrLine[0] == '('){
            i++;
        }
    }

    *road_num = i;                 //包含无效行
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
        get_next_int(&str, &((*road)[i].length));
        get_next_int(&str, &((*road)[i].limit));
        get_next_int(&str, &((*road)[i].lanes_num));
        get_next_int(&str, &((*road)[i].cross_id_start));
        get_next_int(&str, &((*road)[i].cross_id_end));
        get_next_int(&str, &((*road)[i].bothway));
        (*road)[i].capacity = (*road)[i].lanes_num * (*road)[i].length;
        (*road)[i].forward_flow_num = 0;
        (*road)[i].back_flow_num = 0;
        new_a_road_road_que(&((*road)[i]));
        road_map[(*road)[i].id] = (*road)[i];
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

void sort_car_by_speed(Car *car, int car_num){          //按照速度排序速度慢的在前
    int car_sort[11];
    int i;
    CarList *p;
    Car ** car_sort_by_speed = (Car **)malloc(sizeof(Car*)*(car_num));
    carlist = (CarList *)malloc(sizeof(CarList));
    p = carlist;
    car_sort[0] = car_classified[0];
    for(i = 1; i < 11; i++){
		car_sort[i] = car_classified[i-1] + car_sort[i-1];
	}
	for(i = 0; i < car_num; i++){
		car_sort_by_speed[car_sort[car[i].speed]] = &(car[i]);
		car_sort[car[i].speed]++;
	}

    p->car = car_sort_by_speed[0];
    p->last = NULL;
    for(i = 1; i < car_num; i++){               //把排序好的车子 装到链表里
		p->next = (CarList *)malloc(sizeof(CarList));
        p->next->last = p;
        p=p->next;
        p->last;
        p->car =  car_sort_by_speed[i];
        p->next=NULL;
	}
}

void delete_car_from_list(CarList **p){
    CarList *x;
    x = *p;
    if(carlist==(*p)){
        carlist = carlist->next;
    } else {
        (*p)->last = (*p)->next;
    }
    (*p) = (*p)->next;
    free(x);
}


void new_a_road_road_que(Road *road){                    //建立道路供车辆行驶
    int i, j;
    if(road->bothway == 1){
        road->forward = (Road_que *)malloc(sizeof(Road_que)); 
        road->back = (Road_que *)malloc(sizeof(Road_que)); 
        road->forward->room = (Car ***)malloc(sizeof(Car**)*road->lanes_num);  
        road->back->room = (Car ***)malloc(sizeof(Car**)*road->lanes_num);  
        for(i = 0;i < road->lanes_num; i++){
            road->forward->room[i] = (Car **)malloc(sizeof(Car)*road->length);
            road->back->room[i] = (Car **)malloc(sizeof(Car)*road->length);
            for(j = 0; j < road->length; j++)
            {
                road->forward->room[i][j] = NOCAR;
                road->back->room[i][j] = NOCAR;
            }  
        }
    } else {
        road->forward = (Road_que *)malloc(sizeof(Road_que)); 
        road->forward->room = (Car ***)malloc(sizeof(Car**)*road->lanes_num);  
        for(i = 0;i < road->lanes_num; i++){
            road->forward->room[i] = (Car **)malloc(sizeof(Car*)*road->length);
            for(j = 0; j < road->length; j++)
            {
                road->forward->room[i][j] = NOCAR;
            }  
        }
    }
}
