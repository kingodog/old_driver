#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *val;
    int length;
    int max;
} array_t;

void counting_sort(array_t *arr, int *sorted_arr){
    int range = arr->max + 1;
    int temp_arr[range];
    int i, j, k;
    for(i = 0; i < range; i++){
        temp_arr[i] = 0;
    }

    for(j = 0; j < arr->length; j++){
        temp_arr[arr->val[j]]++; 
    }

    for(i = 1; i < range; i++){
        temp_arr[i] += temp_arr[i-1];
    }

    for(k = arr->length - 1; k >= 0; k--){
        sorted_arr[temp_arr[arr->val[k]]-1] = arr->val[k];
        temp_arr[arr->val[k]]--;
    }
}

//要求：arr内元素都在[0，k]之间整数
int main(int argc, char *args[]){
    array_t array;
    int arr[] = {2, 5, 3, 0, 2, 3, 0, 3};
    array.length = 8;
    array.max = 5;
    array.val = arr;
    int sorted_arr[array.length];

    counting_sort(&array, sorted_arr);

    for(int i = 0; i < array.length; i++){
        printf("%d ", sorted_arr[i]);
    }

    return 0;
}

