#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define PARENT(i, d) ((i - 1) / d)
#define CHILD(i, d, k) (d * i + k)

typedef struct{
    int *array;
    int d;
    int length;
    int size;
} heap_t;

void max_d_heapify(heap_t *heap, int i){
    int largest = i;

    for(int k = 0; k < heap->d; k++){
        int child = CHILD(i, heap->d, k);
        if(i < heap->size && heap->array[child] > heap->array[largest]){
            largest = child;
        }
    }

    if(largest != i){
        int temp = heap->array[i];
        heap->array[i] = heap->array[largest];
        heap->array[largest] = temp;

        max_d_heapify(heap, largest);
    }
}

int extract_max(heap_t *heap){
    int max = heap->array[0];
    heap->array[0] = heap->array[heap->size-1];
    heap->size--;
    max_d_heapify(heap, 0);

    return max;
}

void increase_key(heap_t *heap, int i, int key){
    if(key < heap->array[i]){
        fprintf(stderr, "new key is smaller than current key\n");
        exit(0);
    }

    heap->array[i] = key;
    while(i > 0 && heap->array[i] > heap->array[PARENT(i, heap->d)]){
        int temp = heap->array[i];
        heap->array[i] = heap->array[PARENT(i, heap->d)];
        heap->array[PARENT(i, heap->d)] = temp;

        i = PARENT(i, heap->d);
    }
    //插入排序式
    // while (i > 0 && heap->array[PARENT(i, heap->d)] < key) {
    //     heap->array[i] = heap->array[PARENT(i, heap->d)];
    //     i = PARENT(i, heap->d);
    // }

    // heap->array[i] = key;
}

void insert(heap_t *heap, int key){
    if(heap->length == heap->size){
        fprintf(stderr, "heap overflow");
        exit(0);
    }

    heap->size++;
    heap->array[heap->size-1] = INT_MIN;
    increase_key(heap, heap->size-1, key);
}