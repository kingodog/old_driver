#include <stdlib.h>
#include <stdio.h>

/////////////////////////////////////////////////////////////////////////////
// List operations
/////////////////////////////////////////////////////////////////////////////

typedef struct _node_t node_t;
typedef struct _node_t {
    int key;
    node_t *next;
} node_t;

node_t *init_node(int key){
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    if(!new_node){
        printf("init node error!\n");
        return NULL;
    }
    new_node->key = key;
    new_node->next = NULL;
    return new_node;
}

node_t *insert_sorted(node_t *list, int key){
    node_t *node = init_node(key);
    if(!list || list->key > key){
        node->next = list;
        return node;
    }
    node_t *curr = list;
    while(curr->next && curr->next->key < key){
        curr = curr->next;
    }
    node->next = curr->next;
    curr->next = node;
    return list;
}

node_t *delete_key(node_t *list, int key){
    node_t *match;
    if(list && list->key == key){
        match = list;
        list = list->next;
        free(match);
    }
    if(!list){
        return NULL;
    }
    node_t *curr = list;
    while(curr->next){
        if(curr->next->key == key){
            match = curr->next;
            curr->next = match->next;
            free(match);
        }else{
            curr = curr->next;
        }
    }
    return list;
}

node_t *prepend(node_t *list, int key){
    node_t *node = init_node(key);
    node-> next = list;
    return node;
}

int find_min(node_t *list){
    int min = list->key;
    node_t *curr = list->next;
    while(curr){
        if(curr->key < min){
            min = curr->key;
        }
        curr = curr->next;
    }
    return min;
}

node_t *link_together(node_t *list_a, node_t *list_b){
    node_t *list;
    if(!list_a){
        list = list_b;
    }else{
        list = list_a;
        while(list_a->next){
            list_a = list_a->next;
        }
        list_a->next = list_b;
    }
    return list;
}

node_t *merge_sorted(node_t *list_a, node_t *list_b){
    node_t dummy;
    node_t *new = &dummy;
    while(list_a && list_b){
        if(list_a->key < list_b->key){
            new->next = list_a;
            list_a = list_a->next;
            new = new->next;
        }else{
            new->next = list_b;
            list_b = list_b->next;
            new = new->next;
        }
    }
    if(list_a){
        new->next = list_a;
    }else{
        new->next = list_b;
    }
    return dummy.next;
}



typedef struct {
    node_t *head;
} heap_t;

/////////////////////////////////////////////////////////////////////////////
// 1. Mergreable heaps with sorted list
/////////////////////////////////////////////////////////////////////////////

heap_t *heap_init(){
    heap_t *heap = (heap_t *)malloc(sizeof(heap_t));
    if(!heap){
        printf("init heap error!\n");
        return NULL;
    }
    heap->head = NULL;
    return heap;
}

void heap_insert(heap_t *heap, int key){
    heap->head = insert_sorted(heap->head, key);
}

int heap_min(heap_t *heap){
    return heap->head->key;
}

int heap_extract_min(heap_t *heap){
    int min = heap->head->key;
    node_t *head = heap->head;
    heap->head = heap->head->next;
    free(head);
    head = NULL;
    return min;
}

heap_t *heaps_union(heap_t *ha, heap_t *hb){
    heap_t *heap = heap_init();
    heap->head = merge_sorted(ha->head, hb->head);
    free(ha);
    free(hb);
    ha = NULL;
    hb = NULL;
    return heap;
}


/////////////////////////////////////////////////////////////////////////////
// 2. Mergreable heaps with unsorted lists
/////////////////////////////////////////////////////////////////////////////

heap_t *heap_init_u(){
    heap_t *heap = (heap_t *)malloc(sizeof(heap_t));
    if(!heap){
        printf("init heap error!\n");
        return NULL;
    }
    heap->head = NULL;
    return heap;
}

void heap_insert_u(heap_t *heap, int key){
    heap->head = prepend(heap->head, key);
}

int heap_min_u(heap_t *heap){
    return find_min(heap->head);
}

int heap_extract_min_u(heap_t *heap){
    int min = find_min(heap->head);
    heap->head = delete_key(heap->head, min);
    return min;
}

heap_t *heaps_union_u(heap_t *ha, heap_t *hb){
    heap_t *heap = heap_init_u();
    heap->head = link_together(ha->head, hb->head);
    free(ha);
    free(hb);
    ha = NULL;
    hb = NULL;
    return heap;
}

int main(){
    node_t *node1 = init_node(1);
    node1 = insert_sorted(node1, 5);
    node1 = insert_sorted(node1, 8);
    node_t *tmp = node1;
    while(tmp){
        printf("%d ", tmp->key);
        tmp = tmp->next;
    }
    printf("\n");

    heap_t *heap1 = heap_init();
    heap1->head = node1;
    heap_insert(heap1, 2);
    heap_insert(heap1, 6);
    tmp = heap1->head;
    while(tmp){
        printf("%d ", tmp->key);
        tmp = tmp->next;
    }
    printf("\n");
    printf("min: %d\n", heap_min(heap1));

    heap_t *heap2 = heap_init();
    heap_insert(heap2, 3);
    heap_insert(heap2, 4);
    heap_insert(heap2, 7);

    heap_t *heap = heaps_union(heap1, heap2);
    tmp = heap->head;
    while(tmp){
        printf("%d ", tmp->key);
        tmp = tmp->next;
    }
    printf("\n");



    node1 = init_node(1);
    node1 = insert_sorted(node1, 5);
    node1 = insert_sorted(node1, 8);
    tmp = node1;
    while(tmp){
        printf("%d ", tmp->key);
        tmp = tmp->next;
    }
    printf("\n");

    heap1 = heap_init_u();
    heap1->head = node1;
    heap_insert_u(heap1, 2);
    heap_insert_u(heap1, 6);
    tmp = heap1->head;
    while(tmp){
        printf("%d ", tmp->key);
        tmp = tmp->next;
    }
    printf("\n");
    printf("min: %d\n", heap_min_u(heap1));

    heap2 = heap_init_u();
    heap_insert_u(heap2, 3);
    heap_insert_u(heap2, 4);
    heap_insert_u(heap2, 7);

    heap = heaps_union_u(heap1, heap2);
    tmp = heap->head;
    while(tmp){
        printf("%d ", tmp->key);
        tmp = tmp->next;
    }
    printf("\n");



    return 0;
}
