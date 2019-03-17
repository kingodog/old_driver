#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct _node_t node_t;
typedef struct _node_t {
    int key;
    node_t *next;
} node_t;

typedef struct {
    node_t *nil;
} list_t;

list_t *init_list(){
    list_t *list = (list_t *)malloc(sizeof(list_t));
    if(list == NULL){
        printf("init list error!\n");
        return NULL;
    }
    node_t *nil = (node_t *)malloc(sizeof(node_t));
    if(nil == NULL){
        printf("init nil error!\n");
        return NULL;
    }
    list->nil = nil;
    list->nil->key = 0;
    list->nil->next = list->nil;
    return list;
}

void destroy_list(list_t *list){
    node_t *node = list->nil->next;
    node_t *next;

    while(node != list->nil){
        next = node->next;
        free(node);
        node = NULL;
        node = next;
    }
    free(list->nil);
    list->nil = NULL;
    free(list);
    list = NULL;
}

node_t *search(list_t *list, int key){
    node_t *node = list->nil->next;
    list->nil->key = key;
    while(node->key != key){
        node = node->next;
    }
    if(node == list->nil){
        printf("key is not exist!\n");
        return NULL;
    }else{
        return node;
    }
}

void insert(list_t *list, int key){
    node_t *node = (node_t *)malloc(sizeof(node_t));
    node->next = list->nil->next;
    node->key = key;
    list->nil->next = node;
}

void delete(list_t *list, node_t *del){
    node_t *node = list->nil;
    while(node->next != del){
        node = node->next;
    }
    node->next = del->next;
    free(del);
}

void reverse(list_t *list){
    node_t *node = list->nil->next;
    node_t *prev = list->nil;
    node_t *next;
    while(node != list->nil){
        next = node->next;
        node->next = prev;
        prev = node;
        node = next;
    }
    list->nil->next = prev;
}

int main(int argc, char *args[]){
    list_t *list = init_list();
    if(list == NULL){
        printf("init fail!\n");
        return 1;
    }
    insert(list, 1);
    insert(list, 3);
    insert(list, 5);

    node_t *node = list->nil->next;
    while(node != list->nil){
        printf("%d ", node->key);
        node = node->next;
    }
    printf("\n");

    node = search(list, 1);
    delete(list, node);
    node = search(list, 0);
    node = list->nil->next;
    while(node != list->nil){
        printf("%d ", node->key);
        node = node->next;
    }
    printf("\n");

    insert(list, 7);
    reverse(list);
    node = list->nil->next;
    while(node != list->nil){
        printf("%d ", node->key);
        node = node->next;
    }
    printf("\n");

    destroy_list(list);

    return 0;
}