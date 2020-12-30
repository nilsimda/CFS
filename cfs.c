#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "cfs.h"

struct node *updateMin(struct node *root){
    if(root == NULL){
        return NULL;
    }
    if(root->left == NULL){
        root->val += 10;
        return root;
    }
    else
    {
        return updateMin(root->left);
    }
    
}

struct node *rebuildRBTree(struct node *oldRoot, struct node *newRoot){
    if(oldRoot == NULL)
        return NULL;
    
    newRoot = rebuildRBTree(oldRoot->left, newRoot);
    newRoot = rebuildRBTree(oldRoot->right, newRoot);
    
    struct process *p = oldRoot->data;
    int nVal = oldRoot->val - p->decay;
    //printf("%d\n" , nVal);
    struct node *node = newNode(nVal);
    node->data = oldRoot->data;

    return insertRB(newRoot, node);
}

struct node* cfs(struct process *processes, int time){
    struct node *root = NULL;
    while(processes != NULL){
        struct node *node = newNode(processes->PID);
        node->data = processes;
        root = insertRB(root, node);
        processes = processes->next;
    }
    struct node *iter = NULL;
    struct process *p = NULL;

    for(int i = 0; i < time; i++){
        iter = updateMin (root);
        p = iter->data;
        printf("Time: %d, PID: %d\n", iter->val, p->PID);
        root = rebuildRBTree(root, NULL);
    }
    return root;
}