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
    queue_t * que = (queue_t *)malloc(sizeof(queue_t));
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

void destroy_queue(queue_t *que){
    free(que->arr);
    que->arr = NULL;
    free(que);
    que = NULL;
}

bool is_empty(queue_t *que){
    return que->head == -1;
}

bool is_full(queue_t *que){
    return que->tail == que->head;
}

bool enqueue(queue_t *que, int e){
    if(is_full(que)){
        printf("overflow!\n");
        return false;
    }
    que->arr[que->tail] = e;
    if(que->head == -1){
        que->head = que->tail;
    }
    que->tail = (que->tail + 1) % MAXSIZE;
    return true;
}

bool dequeue(queue_t *que, int *e){
    if(is_empty(que)){
        printf("underflow!\n");
        return false;
    }
    *e = que->arr[que->head];
    printf("dequeue:\t%d\n", *e);
    que->head = (que->head + 1) % MAXSIZE;
    if(que->head == que->tail){
        que->head = -1;
        que->tail = 0;
    }
    return true;
}

int main(int argc, char *args[]){
    int i, dequeue_val;
    queue_t *queue = init_queue();
    if(queue){
        enqueue(queue, 2);
        enqueue(queue, 4);
        enqueue(queue, 6);
    }

    for(i = queue->head; i <= (queue->tail+MAXSIZE-1) % MAXSIZE; i++){
        printf("%d ", queue->arr[i]);
    }
    printf("\n");
    printf("queue %s full\n", is_full(queue) ? "is" : "is not");

    dequeue(queue, &dequeue_val);
    dequeue(queue, &dequeue_val);
    dequeue(queue, &dequeue_val);
    printf("queue %s empty\n", is_empty(queue) ? "is" : "is not");

    destroy_queue(queue);

    return 0;
}
