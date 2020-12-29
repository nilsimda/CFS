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
     if(root == NULL || n == NULL){
         return root;
     }

    else if(root == n){
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
        root->right = rotateLeft (root->right, n);
    }

    else{
        root->left = rotateLeft (root->left, n);
    }
    return root;

}

struct node *rotateRight(struct node *root, struct node *n){
    if(root == NULL || n == NULL){
         return root;
     }

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
        root->right = rotateLeft (root->right, n);
    }

    else{
        root->left = rotateLeft (root->left, n);
    }
    return root;
}

struct node *insert(struct node *root, struct node *newNode){
    if(root == NULL){
        newNode->parent = root->parent;
        return newNode;
    }
    else if(root->val <= newNode->val){
        root->left = insert (root->left, newNode);
    }
    else{
        root->right = insert (root->right, newNode);
    }
    return root;
}

struct node *insertRB(struct node *root, struct node *newNode){
    return insert (root, newNode);

    /*root = insert(root, newNode);
    while ((newNode != root) && (newNode->parent->color == RED)){

        if(newNode->parent == newNode->parent->parent->left){

            struct node *rightUncle = newNode->parent->parent->right;

            if(rightUncle->color == RED){
                newNode->parent->color = BLACK;
                rightUncle->color = BLACK;
                newNode->parent->parent = RED;
                newNode = newNode->parent->parent;
            }
            else{
                if(newNode == newNode->parent->right){
                    newNode = newNode->parent;
                    root = rotateLeft(root, newNode);
                }
                newNode->parent->color = BLACK;
                newNode->parent->parent->color = RED;
                root = rotateRight (root, newNode->parent->parent);
            }
        }
        else{
            if(newNode->parent == newNode->parent->parent->right){

            struct node *leftUncle = newNode->parent->parent->left;

                if(leftUncle->color == RED){
                    newNode->parent->color = BLACK;
                    leftUncle->color = BLACK;
                    newNode->parent->parent = RED;
                    newNode = newNode->parent->parent;
                }
                else{
                    if(newNode == newNode->parent->left){
                        newNode = newNode->parent;
                        root = rotateLeft(root, newNode);
                    }
                    newNode->parent->color = BLACK;
                    newNode->parent->parent->color = RED;
                    root = rotateRight (root, newNode->parent->parent);
                }

            }
        }
    }
    root->color = BLACK;
    return root;*/
}
