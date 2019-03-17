#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAXSIZE 3

typedef struct {
    int head;
    int tail;
    int *arr;
} queue_t;

queue_t *init_queue(){
    queue_t *que = (queue_t *)malloc(sizeof(queue_t));
    if(que == NULL){
        printf("init queue error!\n");
        return NULL;
    }
    int *array = (int *)malloc(sizeof(int) * MAXSIZE);
    if(array == NULL){
        printf("init arr error!\n");
        return NULL;
    }
    que->head = -1;
    que->tail = 0;
    que->arr = array;

    return que;
}

bool is_empty(queue_t *que){
    return que->head == -1;
}

bool is_full(queue_t *que){
    return que->tail == que->head;
}

bool push_head(queue_t *que, int e){
    if(is_full(que)){
        printf("overflow!\n");
        return false;
    }
    if(que->head == -1){
        que->head = que->tail;
    }
    que->head = (que->head+MAXSIZE-1) % MAXSIZE;
    que->arr[que->head] = e;

    return true;
}

bool push_tail(queue_t *que, int e){
    if(is_full(que)){
        printf("overflow!\n");
        return false;
    }
    if(que->head == -1){
        que->head = que->tail;
    }
    que->tail = e;
    que->tail = (que->tail+1) % MAXSIZE;

    return true;
}

bool pop_head(queue_t *que, int *e){
    if(is_empty(que)){
        printf("underflow!\n");
        return false;
    }
    *e = que->arr[que->head];
    printf("pop_head:\t%d\n", *e);
    que->head = (que->head+1+MAXSIZE) % MAXSIZE;
    if(que->head == que->tail){
        que->head = -1;
        que->tail = 0;
    }

    return true;
}

bool pop_tail(queue_t *que, int *e){
    if(is_empty(que)){
        printf("underflow!\n");
        return false;
    }
    *e = que->arr[que->tail];
    printf("pop_tail:\t%d\n", *e);
    que->tail = (que->tail-1+MAXSIZE) % MAXSIZE;
    if(que->tail == que->head){
        que->head = -1;
        que->tail = 0;
    }
    
    return true;
}