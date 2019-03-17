#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct _tree_t tree_t;
typedef struct _tree_t {
    tree_t *left;
    tree_t *right;
    tree_t *parent;
    int key;
} tree_t;

#define MAX_SIZE 10
int keys[MAX_SIZE];
int count = 0;

void reset_storage(){
    count = 0;
}

void store(int key){
    keys[count++] = key;
}

//递归输出树每个结点的key
void print_tree(tree_t *tree){
    store(tree->key);
    if(tree->left){
        print_tree(tree->left);
    }
    if(tree->right){
        rint_tree(tree->right);
    }
}

//循环输出树每个结点的key
void print_tree(tree_t *tree) {
    tree_t *stack[MAX_SIZE];
    int count = 0;

    stack[count++] = tree;

    while (count) {
        tree = stack[--count];

        store(tree->key);

        if (tree->right)
            stack[count++] = tree->right;

        if (tree->left)
            stack[count++] = tree->left;
    }
}

//非递归固定额外空间
void print_tree(tree_t *tree) {
    tree_t *prev;
    prev = 0;

    while (tree) {
        if (prev == tree->parent) {
            store(tree->key);
            prev = tree;
            tree = tree->left  ? tree->left :
                   tree->right ? tree->right :
                                 tree->parent;
        } else if (prev == tree->left && tree->right) {
            prev = tree;
            tree = tree->right;
        } else {
            prev = tree;
            tree = tree->parent;
        }
    }
}