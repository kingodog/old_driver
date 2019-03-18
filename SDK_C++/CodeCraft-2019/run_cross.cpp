#include "run_cross.h"

// void put_car(Car *car, Road *road, Cross *cross){
//     if(road->cross_id_start == cross->id){         //放在正向
//         input_car_forward(road, car);
//     } else {
//         input_car_back(road, car);
//     }
// }

// int input_car_forward(Road *road, Car *car){     //返回为-1，代表无法放入车辆
//     int i, j;
//     int speed = get_min(road->limit, car->speed);
//     for(i = 0; i < road->lanes_num; i++){
//         j = 1;
//         if(road->forward_content[i][0] != NOCAR){
//             continue;
//         } else {
//             while(road->forward_content[i][j] == NOCAR && j < speed){
//                 j++;
//             }
//             road->forward_content[i][j-1] = car;
//             return SUCCESSFUL;
//         } 
//     }
//     return DEFEATED;
// }