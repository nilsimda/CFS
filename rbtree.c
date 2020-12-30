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
    if(root == NULL){
        return root;
    }

    else if(root == n){
        struct node *a = n->right;
        struct node *p = n->parent;

        if(a == NULL)
            return n;

       n->right = a->left;
       a->left = n;
       n->parent = a;

       if(n->right != NULL){
           n->right->parent = n;
       }

       if(p != NULL){
           if(n == p->left){
               p->left = a;
           }
           else if (n == p->right){
               p->right = a;
           }
       } 
       a->parent = p; 
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
        struct node *a = n->left;
        struct node *p = n->parent;

        if(a == NULL)
            return n;

       n->left = a->right;
       a->right = n;
       n->parent = a;

       if(n->left != NULL){
           n->left->parent = n;
       }

       if(p != NULL){
           if(n == p->left){
               p->left = a;
           }
           else if (n == p->right){
               p->right = a;
           }
       } 
       a->parent = p; 
       return a;
    }

    else if (root->val < n->val){
        root->right = rotateRight (root->right, n);
    }

    else{
        root->left = rotateRight (root->left, n);
    }
    return root;
}

struct node *insert(struct node *root, struct node *newNode, struct node *parent){
    if(root == NULL){
        newNode->parent = parent;
        return newNode;
    }
    else if(root->val >= newNode->val){
        root->left = insert (root->left, newNode, root);
    }
    else{
        root->right = insert (root->right, newNode, root);
    }
    return root;
}

struct node *getUncle(struct node *node){
    if(node->parent->parent != NULL){
        if (node->parent == node->parent->parent->left)
            return node->parent->parent->right;
        else {
            return node->parent->parent->left;
        }
    }
    else{
        return NULL;
    }
}

struct node *updateRB(struct node *root, struct node *newNode){
    if(newNode->parent == NULL){
        newNode->color = BLACK;
    }
    else if (newNode->parent->color == BLACK ){
        return root;
    }
    else if(getUncle(newNode) != NULL && getUncle(newNode)->color == RED){
            newNode->parent->color = BLACK;
            getUncle(newNode)->color = BLACK;
            newNode->parent->parent->color = RED;
            root = updateRB (root, newNode->parent->parent);
    }
    else{
        struct node *parent = newNode->parent;
        struct node *grandParent = newNode->parent->parent;

        if(newNode == parent->right && grandParent->left == parent){
            root = rotateLeft(root, parent);
            newNode = newNode->left;
        }
        else if(newNode == parent->left && grandParent->right == parent){
            root = rotateRight (root, parent);
            newNode = newNode->right;
        }
        parent = newNode->parent;
        grandParent = newNode->parent->parent;

        if(newNode == parent->left){
            root = rotateRight(root, grandParent);
        }
        else{
            root = rotateLeft(root, grandParent);
        }
        parent->color = BLACK;
        grandParent->color = RED;
    }
    
    return root;
}

struct node *insertRB(struct node *root, struct node *newNode){
    root = insert (root, newNode, NULL);
    return updateRB (root, newNode);
}

struct node *search(struct node *root, int val){
    if ((root->val > val && root->left == NULL) || (root->val > val && root->left->val <= val)){
        return root;
    }
    else if (root->val < val){
        return search (root->right, val);
    }
    else{
        return search (root->left, val);
    }  
}


