#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAXSIZE 3

//两栈=>队列
typedef struct {
    int arr[MAXSIZE];
    int top;
} stack_t;

typedef struct {
    stack_t *head;
    stack_t *tail;
} queue_t;


bool stack_is_empty(stack_t *stack){
    return stack->top == 0;
}

bool stack_is_full(stack_t *stack){
    return stack->top == MAXSIZE;
}

bool push_stack(stack_t *stack, int e){
    if(stack_is_full(stack)){
        printf("satck overflow!\n");
        return false;
    }
    stack->arr[stack->top++] = e;
    return true;
}

int pop_stack(stack_t *stack){
    if(stack_is_empty(stack)){
        printf("stack underflow!\n");
        exit(1);
    }
    return stack->arr[--stack->top];
}


queue_t *init_queue(){
    queue_t *que = (queue_t *)malloc(sizeof(queue_t));
    if(que == NULL){
        printf("init queue error!\n");
        return NULL;
    }
    stack_t *head_stack = (stack_t *)malloc(sizeof(stack_t));
    stack_t *tail_stack = (stack_t *)malloc(sizeof(stack_t));
    if(head_stack == NULL || tail_stack == NULL){
        printf("init stack error!\n");
        return NULL;
    }
    que->head = head_stack;
    que->tail = tail_stack;
    que->head->top = 0;
    que->tail->top = 0;
    return que;
}

void destroy_queue(queue_t *que){
    free(que->head);
    free(que->tail);
    que->head = NULL;
    que->tail = NULL;
    free(que);
    que = NULL;
}

bool queue_is_empty(queue_t *que){
    return que->head->top == 0 && que->tail->top == 0;
}

bool queue_is_full(queue_t *que){
    return que->head->top == MAXSIZE && que->tail->top == MAXSIZE;
}

bool enqueue(queue_t *que, int e){
    if(queue_is_full(que)){
        printf("queue overflow!\n");
        return false;
    }
    if(stack_is_full(que->tail)){
        while(!stack_is_empty(que->tail)){
            int tmp = pop_stack(que->tail);
            push_stack(que->head, tmp);
        }
    }
    push_stack(que->tail, e);
    return true;
}

int dequeue(queue_t *que){
    if(queue_is_empty(que)){
        printf("queue underflow!\n");
        exit(1);
    }
    if(stack_is_empty(que->head)){
        while(!stack_is_empty(que->tail)){
            int tmp = pop_stack(que->tail);
            push_stack(que->head, tmp);
        }
    }
    return pop_stack(que->head);
}

int main(int argc, char *args[]){
    queue_t *queue = init_queue();
    if(queue){
        enqueue(queue, 2);
        enqueue(queue, 4);
    }
    int i;
    for(i = queue->head->top-1; i >= 0; i--){
        printf("%d ", queue->head->arr[i]);
    }
    for(i = 0; i < queue->tail->top; i++){
        printf("%d ", queue->tail->arr[i]);
    }
    printf("\n");
    printf("dequeue: %d\n", dequeue(queue));
    printf("dequeue: %d\n", dequeue(queue));

    enqueue(queue, 2);
    enqueue(queue, 4);
    enqueue(queue, 6);
    enqueue(queue, 8);
    enqueue(queue, 10);
    enqueue(queue, 12);
    printf("queue %s full\n", queue_is_full(queue) ? "is" : "is not");

    while(!queue_is_empty(queue)){
        printf("dequeue: %d\n", dequeue(queue));
    }
    printf("queue %s empty\n", queue_is_empty(queue) ? "is" : "is not");

    destroy_queue(queue);

    return 0;
}
