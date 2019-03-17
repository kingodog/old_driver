#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAXSIZE 3

typedef struct {
    int array[MAXSIZE];
    int top;
} stack_t;

bool is_empty(stack_t *stack){
    return stack->top == 0;
}

bool is_full(stack_t *stack){
    return stack->top == MAXSIZE;
}

bool push(stack_t *stack, int e){
    if(is_full(stack)){
        printf("overflow!\n");
        return false;
    }
    stack->array[stack->top++] = e;
    return true;
}

bool pop(stack_t *stack, int *e){
    if(is_empty(stack)){
        printf("underflow!\n");
        return false;
    }
    *e = stack->array[--stack->top];
    printf("pop:\t%d\n", *e);
    return true;
}

int main(int argc, char *args[]){
    int i, pop_val;
    stack_t stack;
    stack.top = 0;

    push(&stack, 1);
    push(&stack, 3);
    push(&stack, 5);
    printf("stack %s full\n", is_full(&stack) ? "is" : "is not");

    for(i = 0; i < stack.top; i++){
        printf("%d ", stack.array[i]);
    }
    printf("\n");

    pop(&stack, &pop_val);
    pop(&stack, &pop_val);
    pop(&stack, &pop_val);
    printf("stack %s empty\n", is_empty(&stack) ? "is" : "is not");

    return 0;
}