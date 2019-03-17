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

int main(int argc, char *args[]){
    int a[10] = {3, 2, 5, 0, 7, 5, 5, 8, 6, 0};
    int start = 0, last = 9;
    for(int pos = 1; pos < 11; pos++){
        int ret = randomized_select(a, start, last, pos);
        printf("%d\n", ret);
    }
    
    return 0;
}