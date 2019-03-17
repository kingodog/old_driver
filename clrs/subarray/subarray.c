#include <stdio.h>
#include <stdlib.h>
#define MIN -2e8

typedef struct {
    int left;
    int right;
    int sum;
} max_subarray;

max_subarray find_max_subarray(int *array, int low, int high);

int main(int argc, char *args[]){
    int a[16] = {13, -3, -25, 20, -3, -16, -23, 18, 20, -7, 12, -5, -22, 15, -4, 7};
    max_subarray subarr;

    subarr = find_max_subarray(a, 0, 15);
    
    printf("%d, %d, %d\n", subarr.sum, subarr.left, subarr.right);

    return 0;
}


//暴力解法
// max_subarray find_max_subarray(int *array, int low, int high){
//     int i, j;
//     int sum = MIN;
//     int current_sum = 0;
//     max_subarray max;

//     for(i = low; i < high + 1; i++){
//         current_sum = 0;  
//         for(j = i; j < high + 1; j++){
//             current_sum += array[j];
//             if(current_sum > sum){
//                 max.left = i;
//                 max.right = j;
//                 sum = current_sum;
//             }
//         }
//     }
//     max.sum = sum;

//     return max;
// }


//非递归、线性时间
// max_subarray find_max_subarray(int *array, int low, int high){
//     max_subarray suffixes[high-low+1];
//     suffixes[0].left = low;
//     suffixes[0].right = low;
//     suffixes[0].sum = array[low];
//     max_subarray max = suffixes[0];
//     int i;

//     for(i = 1; i < high - low + 1; i++) {
//         if(suffixes[i-1].sum < 0) {
//             suffixes[i].left = i;
//             suffixes[i].right = i;
//             suffixes[i].sum = array[i];
//         }else{
//             suffixes[i].left = suffixes[i-1].left;
//             suffixes[i].right = i;
//             suffixes[i].sum = suffixes[i-1].sum + array[i];
//         }
//     }
//     for(i = 1; i < high - low + 1; i++) {
//         if(max.sum < suffixes[i].sum){
//             max = suffixes[i];
//         }
//     }

//     return max;
// }


//分治策略
max_subarray find_max_crossing_subarray(int *array, int low, int mid, int high){
    max_subarray max;
    int sum = 0;
    int left_sum = MIN;
    int right_sum = MIN;
    int i;
    for(i = mid; i >= low; i--){
        sum += array[i];
        if(sum > left_sum){
            left_sum = sum;
            max.left = i;
        }
    }
    sum = 0;
    for(i = mid + 1; i <= high; i++){
        sum += array[i];
        if(sum > right_sum){
            right_sum = sum;
            max.right = i;
        }
    }
    max.sum = left_sum + right_sum;

    return max;
}

max_subarray find_max_subarray(int *array, int low, int high){
    max_subarray max_left;
    max_subarray max_right;
    max_subarray max_cross;
    max_subarray max;
    int mid;

    if(low == high){
        max.left = low;
        max.right = high;
        max.sum = array[low];
    }
    else{
        mid = (low + high) / 2;
        max_left = find_max_subarray(array, low, mid);
        max_right = find_max_subarray(array, mid + 1, high);
        max_cross = find_max_crossing_subarray(array, low, mid, high);

        if(max_left.sum >= max_right.sum && max_left.sum >= max_cross.sum){
            max = max_left;
        }
        else if(max_right.sum >= max_left.sum && max_right.sum >= max_cross.sum){
            max = max_right;
        }
        else{
            max = max_cross;
        }
    }

    return max;
}