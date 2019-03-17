#include <stdio.h>
#include <stdlib.h>

void swap(int *arr, int i, int j){
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

int partition(int *arr, int start, int last){
    int i = start;
    int pivot = arr[last];
    for(int j = start; j < last; j++){
        if(arr[j] <= pivot){
            swap(arr, i, j);
            i++;
        }
    }
    swap(arr, i, last);

    return i;
}

void quicksort(int *arr, int start, int last){
    if(start < last){
        int mid = partition(arr, start, last);

        quicksort(arr, start, mid - 1);
        quicksort(arr, mid + 1, last);
    }
}

int main(int argc, char *args[]){
    int a[] = {2, 8, 7, 1, 3, 5, 6, 4};
    int start = 0;
    int last = 7;

    quicksort(a, start, last);
    
    for(int i = 0; i < 8; i++){
        printf("%d ", a[i]);
    }
    
    return 0;
}