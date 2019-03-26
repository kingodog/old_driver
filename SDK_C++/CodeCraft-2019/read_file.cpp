#include "read_file.h"
#include "project.h"
extern CarList *carlist;
extern CarList *carlist_sroted;
int car_classified[11];

extern int ** cross_to_road;

extern hash_map<int, Road *> road_map;
extern hash_map<int, Cross *> cross_map;
extern int map_capacity;
extern int surplus_map_capacity;
void get_imformation(int *car_num, int *cross_num, int *road_num, 
        Car **car, Cross **cross, Road **road, 
        char *car_path, char *cross_path, char *road_path){

    get_road_imformation(road_path, road_num, road);            //不可改变先后顺序
    merge_sort_roads(*road, 0, (*road_num-1));
    get_cross_imformation(cross_path, cross_num, cross);
    merge_sort_crosses(*cross, 0, (*cross_num-1));
    get_car_imformation(car_path, car_num, car, road_num);
    merge_sort_cars(*car, 0, (*car_num-1));
    sort_car_by_speed_and_creat_list(*car, *car_num);
    build_cross_num_to_road_num_matrix(*cross_num, *road_num, *cross, *road);
}

void get_car_imformation(char *path, int *car_num, Car **car, int *road_num){

    FILE *fp1 = fopen(path,"r");
    char StrLine[1024];
    char *str;
    int i=0;
    for(i = 0; i < 11; i++){     //初始化数组;
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
                (*cross)[i].road_dir[k] = NULL;
                (*cross)[i].road_dir_out[k] = NULL;
                (*cross)[i].road[k] = NULL;
            } else {
                (*cross)[i].road[k] = road_map[(*cross)[i].road_id[k]];
                if(road_map[(*cross)[i].road_id[k]]->cross_id_start == (*cross)[i].id){   //特别注意，
                    if(road_map[(*cross)[i].road_id[k]]->bothway == 1){
                        (*cross)[i].road_dir[k] = road_map[(*cross)[i].road_id[k]]->back;
                        (*cross)[i].road_dir_out[k] = road_map[(*cross)[i].road_id[k]]->forward;
                    } else {
                        (*cross)[i].road_dir[k] = NULL;
                        (*cross)[i].road_dir_out[k] = road_map[(*cross)[i].road_id[k]]->forward;
                    }
                } else {
                    if(road_map[(*cross)[i].road_id[k]]->bothway == 1){
                        (*cross)[i].road_dir[k] = road_map[(*cross)[i].road_id[k]]->forward;
                        (*cross)[i].road_dir_out[k] = road_map[(*cross)[i].road_id[k]]->back;
                    } else {
                        (*cross)[i].road_dir[k] = road_map[(*cross)[i].road_id[k]]->forward;
                        (*cross)[i].road_dir_out[k] = NULL;

                    }
                }



            }
        }
        
        cross_map[(*cross)[i].id] = &(*cross)[i];


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

    cross->total_que = 0;                       //初始化排序好的邻接的que
    for(i = 0; i < cross->total_road; i++){                  
        if(road_map[cross->road_id_sorted[i]]->cross_id_start == cross->id){
            if(road_map[cross->road_id_sorted[i]]->back != NULL){
                cross->road_dir_sorted[cross->total_que] = road_map[cross->road_id_sorted[i]]->back;
                cross->total_que++;
            }
        } else {
            if(road_map[cross->road_id_sorted[i]]->forward != NULL){
                cross->road_dir_sorted[cross->total_que]  = road_map[cross->road_id_sorted[i]]->forward;
                cross->total_que++;
            }
        }
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

    map_capacity = 0;

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

        (*road)[i].forward_surplus_flow = (*road)[i].capacity;
        (*road)[i].pre_forward_surplus_flow = (*road)[i].capacity;
        if((*road)[i].bothway == 1){
            map_capacity += (*road)[i].capacity*2;         //调整全体地图的全体容量
            (*road)[i].back_surplus_flow = (*road)[i].capacity;
            (*road)[i].pre_back_surplus_flow = (*road)[i].capacity;
        } else {
            map_capacity += (*road)[i].capacity;
            (*road)[i].back_surplus_flow = 0;
            (*road)[i].pre_back_surplus_flow = 0;
        }
        new_a_road_road_que(&((*road)[i]));
        road_map[(*road)[i].id] = &(*road)[i];
        i++;
    }

    surplus_map_capacity = map_capacity;        //剩余容量的初始化
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

void sort_car_by_speed_and_creat_list(Car *car, int car_num){          //按照速度排序速度慢的在前
    int car_sort[11];
    int i;

    CarList *p;
    CarList *p_sorted;

    Car ** car_sort_by_speed = (Car **)malloc(sizeof(Car*)*(car_num));
    carlist = (CarList *)malloc(sizeof(CarList));
    carlist_sroted = (CarList *)malloc(sizeof(CarList));

    p = carlist;                                        //一个按照速度排序，一个没有
    p_sorted = carlist_sroted;                  

    car_sort[0] = car_classified[0];
    for(i = 1; i < 11; i++){
		car_sort[i] = car_classified[i-1] + car_sort[i-1];
	}
	for(i = 0; i < car_num; i++){
		car_sort_by_speed[car_sort[car[i].speed]] = &(car[i]);
		car_sort[car[i].speed]++;
	}

    p_sorted->car = car_sort_by_speed[0];
    p_sorted->last = NULL;

    p->car = &(car[0]);
    p->last = NULL;

    for(i = 1; i < car_num; i++){               //装到对应链表里
		p_sorted->next = (CarList *)malloc(sizeof(CarList));
        p_sorted->next->last = p_sorted;
        p_sorted = p_sorted->next;
        p_sorted->car =  car_sort_by_speed[i];
        p_sorted->next=NULL;


        p->next = (CarList *)malloc(sizeof(CarList));
        p->next->last = p;
        p = p->next;
        p->car =  &(car[i]);
        p->next=NULL;
	}
}

void delete_car_from_list(CarList **p){
    CarList *x;
    x = *p;
    if(*p == NULL){
        return;
    }
    if(carlist==(*p)){
        carlist = carlist->next;
        if(carlist != NULL){
            carlist->last = NULL;
        }
    } else {
        (*p)->last->next = (*p)->next;
        if((*p)->next != NULL){
            (*p)->next->last = (*p)->last;
        }
    }
    (*p) = (*p)->next;
    free(x);
}


void new_a_road_road_que(Road *road){                    //建立道路供车辆行驶
    if(road->bothway == 1){
        road->forward = init_road_que(road->length, road->lanes_num);
        road->back = init_road_que(road->length, road->lanes_num);
    } else {
        road->forward = init_road_que(road->length, road->lanes_num);
        road->back = NULL;
    }
}



void merge_roads(Road *array, int left, int mid, int right){
    int i = 0, j = 0, k = 0;
    Road *tmp_l = (Road *)malloc(sizeof(Road) * (mid - left + 1));
    Road *tmp_r = (Road *)malloc(sizeof(Road) * (right - mid));

    for(i = 0; i < mid - left + 1; i++){
        tmp_l[i] = array[left + i];
    }
    for(j = 0; j < right - mid; j++){
        tmp_r[j] = array[mid + j + 1];
    }
    i = j = 0;
    for(k = left; i < mid - left + 1 && j < right - mid; k++){
        if(tmp_l[i].id > tmp_r[j].id){
            array[k] = tmp_r[j++];
        }else{
            array[k] = tmp_l[i++];
        }
    }
    while(i < mid - left + 1){
        array[k++] = tmp_l[i++];
    }
    while(j < right - mid){
        array[k++] = tmp_r[j++];
    }

    free(tmp_l);
    free(tmp_r);
}


void merge_sort_roads(Road *array, int left, int right){
    int mid = 0;
    if(left < right){
        mid = (left + right) / 2;
        merge_sort_roads(array, left, mid);
        merge_sort_roads(array, mid + 1, right);
        merge_roads(array, left, mid, right);
    }
}


void merge_crosses(Cross *array, int left, int mid, int right){
    int i = 0, j = 0, k = 0;
    Cross *tmp_l = (Cross *)malloc(sizeof(Cross) * (mid - left + 1));
    Cross *tmp_r = (Cross *)malloc(sizeof(Cross) * (right - mid));

    for(i = 0; i < mid - left + 1; i++){
        tmp_l[i] = array[left + i];
    }
    for(j = 0; j < right - mid; j++){
        tmp_r[j] = array[mid + j + 1];
    }
    i = j = 0;
    for(k = left; i < mid - left + 1 && j < right - mid; k++){
        if(tmp_l[i].id > tmp_r[j].id){
            array[k] = tmp_r[j++];
        }else{
            array[k] = tmp_l[i++];
        }
    }
    while(i < mid - left + 1){
        array[k++] = tmp_l[i++];
    }
    while(j < right - mid){
        array[k++] = tmp_r[j++];
    }

    free(tmp_l);
    free(tmp_r);
}


void merge_sort_crosses(Cross *array, int left, int right){
    int mid = 0;
    if(left < right){
        mid = (left + right) / 2;
        merge_sort_crosses(array, left, mid);
        merge_sort_crosses(array, mid + 1, right);
        merge_crosses(array, left, mid, right);
    }
}

void merge_cars(Car *array, int left, int mid, int right){
    int i = 0, j = 0, k = 0;
    Car *tmp_l = (Car *)malloc(sizeof(Car) * (mid - left + 1));
    Car *tmp_r = (Car *)malloc(sizeof(Car) * (right - mid));

    for(i = 0; i < mid - left + 1; i++){
        tmp_l[i] = array[left + i];
    }
    for(j = 0; j < right - mid; j++){
        tmp_r[j] = array[mid + j + 1];
    }
    i = j = 0;
    for(k = left; i < mid - left + 1 && j < right - mid; k++){
        if(tmp_l[i].id > tmp_r[j].id){
            array[k] = tmp_r[j++];
        }else{
            array[k] = tmp_l[i++];
        }
    }
    while(i < mid - left + 1){
        array[k++] = tmp_l[i++];
    }
    while(j < right - mid){
        array[k++] = tmp_r[j++];
    }

    free(tmp_l);
    free(tmp_r);
}


void merge_sort_cars(Car *array, int left, int right){
    int mid = 0;
    if(left < right){
        mid = (left + right) / 2;
        merge_sort_cars(array, left, mid);
        merge_sort_cars(array, mid + 1, right);
        merge_cars(array, left, mid, right);
    }
}

void build_cross_num_to_road_num_matrix(int cross_num, int road_num, Cross *cross, Road *road){
    int i, j, k, l;
    cross_to_road = new_a_int_matrix(cross_num);
    for( i = 0; i < cross_num; i++){
       for( j = 0; j < cross_num; j++){
            cross_to_road[i][j] = NIL;
            cross_to_road[j][i] = NIL;
            if(i  == j){
                continue;
            }  
            for(k = 0; k < 4; k++){
                for( l = 0; l < 4; l++)
                {
                    if(cross[i].road_id[k] == cross[j].road_id[l]&& cross[i].road_id[k] != NIL){
                        cross_to_road[i][j] = cross[i].road_id[k];
                        cross_to_road[j][i] = cross[i].road_id[k];
                        break;                   //节约点点时间
                    }
                }
           }
       }
    }
    return;
}
