#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "rbtree.h"

struct node *newNode (int val){
    struct node node = {val, RED, NULL, NULL, NULL, NULL};
    return &node;
}

struct node *rotateLeft(struct node *root, struct node *n){
    if(root == n){
       struct node *a = n->right;
       a->parent = n->parent;
       n->right = a->left;
       a->left = n;
       n->parent = a;

    }

    else if (root->val < n->val){
        rotateLeft (root->right, n);
    }

    else{
        rotateRight (root->left, n);
    }

}

struct node *rotateRight(struct node *root, struct node *n){
     if(root == n){
        struct node *b = n->left;
        b->parent = n->parent;
        n->left = b->right;
        b->right = n;
        n->parent = b;
    }

    else if (root->val < n->val){
        rotateLeft (root->right, n);
    }

    else{
        rotateRight (root->left, n);
    }
}