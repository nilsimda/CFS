#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "rbtree.h"

struct node *newNode (int val){
    struct node *node = malloc (sizeof(struct node));

    if(node == NULL)
        return NULL;

    node->val    = val;
    node->color  = RED;
    node->data   = NULL;
    node->left   = NULL;
    node->right  = NULL;
    node->parent = NULL;

    return node;
      
}

struct node *rotateLeft(struct node *root, struct node *n){
    if(root == n){
        if(n->right == NULL)
            return n;

       struct node *a = n->right;
       a->parent = n->parent;
       n->right = a->left;
       a->left = n;
       n->parent = a;
       return a;
    }

    else if (root->val < n->val){
        return rotateLeft (root->right, n);
    }

    else{
        return rotateRight (root->left, n);
    }

}

struct node *rotateRight(struct node *root, struct node *n){
     if(root == n){
        if(n->left == NULL)
            return n;

        struct node *b = n->left;
        b->parent = n->parent;
        n->left = b->right;
        b->right = n;
        n->parent = b;
        return b;
    }

    else if (root->val < n->val){
        return rotateLeft (root->right, n);
    }

    else{
        return rotateRight (root->left, n);
    }
}

struct node *insertRB(struct node *root, struct node *newNode){
    return NULL;
}