#include <stdio.h>
#include <stdlib.h>

int median_index(int n){
    if(n % 2){
        return n / 2;
    }else{
        return n / 2 - 1;
    }
}

int two_array_median(int *a, int *b, int len){
    if(len == 1){
        return a[0] <= b[0] ? a[0] : b[0];
    }
    int mid = median_index(len);
    int half_size = mid + 1;
    int length = len - half_size;
    int i;
    if(a[mid] <= b[mid]){
        int a_greater[length], b_lesser[length];
        for(i = 0; i < length; i++){
            a_greater[i] = a[half_size+i];
            b_lesser[i] = b[i];
        }
        return two_array_median(a_greater, b_lesser, length);
    }else{
        int a_lesser[length], b_greater[length];
        for(i = 0; i < length; i++){
            a_lesser[i] = a[i];
            b_greater[i] = b[half_size+i];
        }
        return two_array_median(a_lesser, b_greater, length);
    }
}

int main(int argc, char *args[]){
    int a[] = {0, 2, 3, 7, 8, 12, 15, 18, 20, 25};
    int b[] = {0, 1, 3, 4, 6, 13, 15, 22, 24, 24};
    int length = sizeof(a) / sizeof(int);

    int i = two_array_median(a, b, length);
    
    printf("%d\n", i);

    return 0;
}