#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "cfs.h"
#include "utility.h"

struct node *findMin(struct node *root){
    if(root == NULL){
        return NULL;
    }
    if(root->left == NULL){
        return root;
    }
    else {
        return findMin(root->left);
    }    
}

struct node *rebuildTree(struct node *root, struct node *ins){
    if(root == NULL){
        return NULL;
    }

    struct process *p = root->data;
    struct node *n = newNode(root->val - p->decay);
    n->data = p;
    ins = insertRB (ins, n);

    if(root->left != NULL){
        ins = rebuildTree(root->left, ins);
    }
    if(root->right != NULL){
        ins = rebuildTree(root->right, ins);
    }

    return ins;
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
        iter = findMin (root);
        p = iter->data;
        printf("Time: %d PID: %d\n", iter->val, p->PID);
        iter->val += 10;
        root = rebuildTree(root, NULL);
    }
    return root;
}