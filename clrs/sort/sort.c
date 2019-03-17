#include <stdio.h>
#include <stdlib.h>

void sort(int *array);

int main(int argc, char *args[]){
    int a[10] = {56, 32, 95, 8, 14, 87, 26, 43, 28, 64};
    int i = 0;

    sort(a);
    
    for(i = 0; i < 10; i++){
        printf("%d ", a[i]);
    }
    
    getchar();
    return 0;
}

//插入排序
// void sort(int *array){
//     int i = 0, prev_id = 0, curr = 0;
//     for(i = 1; i < 10; i++){
//         curr = array[i];
//         prev_id = i - 1;
//         while(prev_id >= 0 && array[prev_id] > curr){
//             array[prev_id+1] = array[prev_id];
//             --prev_id;
//         }
//         array[prev_id+1] = curr;
//     }
// }


//冒泡排序
// void sort(int *array){
//     int i = 0, j = 0, temp = 0;
//     for(i = 0; i < 10 - 1; i++){
//         for(j = 0; j < 10 - 1 - i; j++){
//             if(array[j] > array[j+1]){
//                 temp = array[j+1];
//                 array[j+1] = array[j];
//                 array[j] = temp;
//             }
//         }
//     }
// }


//选择排序
// void sort(int *array){
//     int i = 0, j = 0, min_id = 0, temp = 0;
//     for(i = 0; i < 10 - 1; i++){
//         min_id = i;
//         for(j = i + 1; j < 10; j++){
//             if(array[min_id] > array[j]){
//                 min_id = j;
//             }
//         } 
//         temp = array[i];
//         array[i] = array[min_id];
//         array[min_id] = temp;
//     }
// }


//归并排序
void merge_sort(int *array, int left, int right);
void merge(int *array, int left, int mid, int right);

void sort(int *array){
    merge_sort(array, 0, 9);
}

void merge_sort(int *array, int left, int right){
    int mid = 0;
    if(left < right){
        mid = (left + right) / 2;
        merge_sort(array, left, mid);
        merge_sort(array, mid + 1, right);
        merge(array, left, mid, right);
    }
}

void merge(int *array, int left, int mid, int right){
    int i = 0, j = 0, k = 0;
    int temp_l[mid - left + 1], temp_r[right - mid];
    for(i = 0; i < mid - left + 1; i++){
        temp_l[i] = array[left + i];
    }
    for(j = 0; j < right - mid; j++){
        temp_r[j] = array[mid + j + 1];
    }
    i = j = 0;
    for(k = left; i < mid - left + 1 && j < right - mid; k++){
        if(temp_l[i] > temp_r[j]){
            array[k] = temp_r[j++];
        }else{
            array[k] = temp_l[i++];
        }
    }
    while(i < mid - left + 1){
        array[k++] = temp_l[i++];
    }
    while(j < right - mid){
        array[k++] = temp_r[j++];
    }
}


