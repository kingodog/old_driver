#include <stdio.h>
#include <stdlib.h>

void swap(int *arr, int i, int j){
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

int hoare_partition(int A[], int p, int r){
    int x = A[p],
        i = p - 1,
        j = r + 1;

    while(1){
        do{j--;} while(!(A[j] <= x));
        do{i++;} while(!(A[i] >= x));

        if(i < j){
            swap(A, i, j);
        }else{
            return j;
        }
    }
}

void quicksort(int A[], int p, int r){
    if(p < r){
        int q = hoare_partition(A, p, r);
        quicksort(A, p, q);
        quicksort(A, q + 1, r);
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



