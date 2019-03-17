#include <stdio.h>
#include <stdlib.h>

void swap(int *arr, int i, int j){
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

int partition(int *arr, int start, int last){
    int pivot = arr[last];
    int i = start, j = start;
    for(i = start; i < last; i++){
        if(arr[i] <= pivot){
            swap(arr, i, j);
            j++;
        }
    } 
    swap(arr, j, last);

    return j;
}

int randomized_partition(int *arr, int start, int last) {
    int i = rand() % (last-start) + start;

    swap(arr, i, last);

    return partition(arr, start, last);
}

int randomized_select(int *arr, int start, int last, int pos){
    if(start == last){
        return arr[start];
    }
    int q = randomized_partition(arr, start, last);
    int k = q - start + 1;

    if(pos == k){
        return arr[q];
    }
    else if(pos < k){
        return randomized_select(arr, start, q-1, pos);
    }
    else{
        return randomized_select(arr, q+1, last, pos-k);
    }
}

void near_median(int *arr, int *result, int len, int k){
    int mid = randomized_select(arr, 0, len-1, len/2);
    int distance[len];
    int i, j = 0;

    for(i = 0; i < len; i++){
        distance[i] = abs(arr[i] - mid);
    }

    int limit = randomized_select(distance, 0, len-1, k);

    for(i = 0; i < len; i++){
        if(abs(arr[i] - mid) < limit){
            result[j++] = arr[i];
        }
    }
    for(i = 0; j <= k && i < len; i++){
        if(abs(arr[i] - mid) == limit){
            result[j++] = arr[i];
        }
    }
}

int main(int argc, char *args[]){
    int a[] = {3, 8, 13, 6, 9, 5, 0, 12, 7, 1};
    int length = sizeof(a) / sizeof(int);
    int k = 7;
    int result[k];
    near_median(a, result, length, k);

    for(int i = 0; i < k; i++){
        printf("%d\n", result[i]);
    }

    return 0;
}