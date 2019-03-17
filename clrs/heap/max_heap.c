#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define PARENT(i) ((i - 1) / 2)
#define LEFT(i) (2 * i + 1)
#define RIGHT(i) (2 * i + 2)

typedef struct{
    int *array;
    int length;
    int size;
} heap_t;

void max_heapify(heap_t *heap, int i){
    int left, right, largest;
    
    //递归
    //////////////////////////////////////////
    left = LEFT(i);
    right = RIGHT(i);

    if(left < heap->size && heap->array[left] > heap->array[i]){
        largest = left;
    }else{
        largest = i;
    }
    if(right < heap->size && heap->array[right] > heap->array[largest]){
        largest = right;
    }
    
    if(largest != i){
        int temp = heap->array[i];
        heap->array[i] = heap->array[largest];
        heap->array[largest] = temp;

        max_heapify(heap, largest);
    }
    //////////////////////////////////////////
    
    //非递归
    //////////////////////////////////////////
    // while(1){
    //     left = LEFT(i);
    //     right = RIGHT(i);

    //     if(left < heap->size && heap->array[left] > heap->array[i]){
    //         largest = left;
    //     }else{
    //         largest = i;
    //     }
    //     if(right < heap->size && heap->array[right] > heap->array[largest]){
    //         largest = right;
    //     }

    //     if(largest == i){
    //         return;
    //     }

    //     int temp = heap->array[i];
    //     heap->array[i] = heap->array[largest];
    //     heap->array[largest] = temp;

    //     i = largest;
    // }
    //////////////////////////////////////////
}

//自底向上方法建堆
void build_max_heap(heap_t *heap){
    heap->size = heap->length;
    int i;
    for(i = heap->length/2-1; i >= 0; i--){
        max_heapify(heap, i);
    }
}

void heap_sort(heap_t *heap){
    build_max_heap(heap);
    int i;
    for(i = heap->length-1; i >= 1; i--){
        int temp = heap->array[i];
        heap->array[i] = heap->array[0];
        heap->array[0] = temp;

        heap->size--;
        max_heapify(heap, 0);
    }
}

int heap_maximum(heap_t *heap){
    return heap->array[0];
}

int heap_extract_max(heap_t *heap){
    if(heap->size == 0){
        fprintf(stderr, "heap underflow\n");
        exit(0);
    }

    int max = heap->array[0];
    heap->array[0] = heap->array[heap->size-1];
    heap->size--;
    max_heapify(heap, 0);

    return max;
}

void heap_increase_key(heap_t *heap, int i, int key){
    if(key < heap->array[i]){
        fprintf(stderr, "new key is smaller than current key\n");
        exit(0);
    }

    heap->array[i] = key;
    while(i > 0 && heap->array[i] > heap->array[PARENT(i)]){
        int temp = heap->array[i];
        heap->array[i] = heap->array[PARENT(i)];
        heap->array[PARENT(i)] = temp;

        i = PARENT(i);
    }
}

void max_heap_insert(heap_t *heap, int key){
    if(heap->length == heap->size){
        fprintf(stderr, "heap overflow");
        exit(0);
    }

    heap->size++;
    heap->array[heap->size-1] = INT_MIN;
    heap_increase_key(heap, heap->size-1, key);
}

//插入方法建堆
void build_max_heap_2(heap_t *heap){
    heap->size = 0;
    int i;
    for(i = 0; i < heap->length; i++){
        max_heap_insert(heap, heap->array[i]);
    }
}


int main(int argc, char *args[]){
    int a[] = {1, 2, 3, 4, 5, 6};
    heap_t *heap;
    heap = (heap_t *)malloc(sizeof(heap_t));
    heap->array = a;
    heap->length = 6;
    heap->size = 0;

    build_max_heap(heap);
    int i;
    for(i = 0; i < heap->length; i++){
        printf("%d ", heap->array[i]);
    }

    printf("\n");

    int max = heap_extract_max(heap);
    printf("max: %d\tsize: %d\n", max, heap->size);

    max = heap_maximum(heap);
    printf("max: %d\n", max);

    max_heap_insert(heap, 6);
    heap_sort(heap);
    for(i = 0; i < heap->length; i++){
        printf("%d ", heap->array[i]);
    }

    free(heap);
    heap = NULL;

    return 0;
}