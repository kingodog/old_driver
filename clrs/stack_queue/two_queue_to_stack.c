#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAXSIZE 3

//两队列=>栈
typedef struct {
    int head;
    int tail;
    int arr[MAXSIZE];
} queue_t;

typedef struct {
    queue_t *que1;
    queue_t *que2;
} stack_t;


bool queue_is_empty(queue_t *que){
    return que->head == -1;
}

bool queue_is_full(queue_t *que){
    return que->tail == que->head;
}

bool enqueue(queue_t *que, int e){
    if(queue_is_full(que)){
        printf("queue overflow!\n");
        return false;
    }
    que->arr[que->tail] = e;
    if(que->head == -1){
        que->head = que->tail;
    }
    que->tail = (que->tail+1) % MAXSIZE;
    return true;
}

int dequeue(queue_t *que){
    if(queue_is_empty(que)){
        printf("underflow!\n");
        return false;
    }
    int tmp = que->arr[que->head];
    que->head = (que->head+1) % MAXSIZE;
    if(que->head == que->tail){
        que->head = -1;
        que->tail = 0;
    }
    return tmp;
}



stack_t *init_stack(){
    stack_t *stack = (stack_t *)malloc(sizeof(stack_t));
    if(stack == NULL){
        printf("init stack error!\n");
        return NULL;
    }
    queue_t *queue_in = (queue_t *)malloc(sizeof(queue_t));
    queue_t *queue_out = (queue_t *)malloc(sizeof(queue_t));
    if(queue_in == NULL || queue_out == NULL){
        printf("init queue error!\n");
        return NULL;
    }
    queue_in->head = queue_out->head = -1;
    queue_in->tail = queue_out->tail = 0;
    stack->que1 = queue_in;
    stack->que2 = queue_out;
    return stack;
}

void destroy_stack(stack_t *stack){
    free(stack->que1);
    free(stack->que2);
    stack->que1 = NULL;
    stack->que2 = NULL;
    free(stack);
    stack = NULL;
}

bool stack_is_empty(stack_t *stack){
    return stack->que1->head == -1 && stack->que2->head == -1;
}

bool stack_is_full(stack_t *stack){
    return stack->que1->tail == stack->que1->head || stack->que2->tail == stack->que2->head;
}

bool push_stack(stack_t *stack, int e){
    if(stack_is_full(stack)){
        printf("overflow!\n");
        return false;
    }
    if(!queue_is_empty(stack->que1)){
        enqueue(stack->que1, e);
    }else{
        enqueue(stack->que2, e);
    }
    return true;
}

int pop_stack(stack_t *stack){
    if(stack_is_empty(stack)){
        printf("underflow!\n");
        return false;
    }
    queue_t *que_in = NULL, *que_out = NULL;
    if(!queue_is_empty(stack->que1)){
        que_in = stack->que2;
        que_out = stack->que1;
    }else{
        que_in = stack->que1;
        que_out = stack->que2;
    }
    while((que_out->tail+MAXSIZE-1-que_out->head) % MAXSIZE > 0){
        int tmp = dequeue(que_out);
        enqueue(que_in, tmp);
    }
    return dequeue(que_out);
}

int main(int argc, char *args[]){
    stack_t *stack = init_stack();
    if(stack){
        push_stack(stack, 2);
        push_stack(stack, 4);
    }
    int i;
    for(i = 0; i < stack->que1->tail; i++){
        printf("%d ", stack->que1->arr[i]);
    }
    for(i = 0; i < stack->que2->tail; i++){
        printf("%d ", stack->que2->arr[i]);
    }
    printf("\n");
    printf("pop: %d\n", pop_stack(stack));
    printf("pop: %d\n", pop_stack(stack));

    push_stack(stack, 2);
    push_stack(stack, 4);
    push_stack(stack, 6);
    push_stack(stack, 8);
    printf("stack %s full\n", stack_is_full(stack) ? "is" : "is not");

    while(!stack_is_empty(stack)){
        printf("pop: %d\n", pop_stack(stack));
    }
    printf("stack %s empty\n", stack_is_empty(stack) ? "is" : "is not");

    destroy_stack(stack);

    return 0;
}