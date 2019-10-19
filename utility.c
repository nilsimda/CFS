#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "utility.h"

void print_val(struct node *node) {
    printf("node->val = %d\n", node->val);
}


void free_node(struct node *node)
{
    if ( node )
    {
        free(node);
    }
}

void iterate_in_order(struct node *node,
                      void (*f)(struct node *n)) {
    if (node == NULL)
        return;

    iterate_in_order(node->left, f);
    f(node);
    iterate_in_order(node->right, f);
}

void iterate_post_order(struct node *node,
                        void (*f)(struct node *n))
{
    if ( node == NULL )
        return;

    iterate_post_order(node->left, f);
    iterate_post_order(node->right, f);
    f(node);
}

struct listItem {
    struct node* data;
    struct listItem* next;
};

struct listItem* pushBack(struct listItem* start, struct node* data) {
    struct listItem* newItem = (struct listItem*)malloc(sizeof(struct listItem));
    CHECK_ALLOC(newItem);
    newItem->data = data;
    newItem->next = NULL;

    if (start == NULL) {
        return newItem;
    }
    struct listItem* iter = start;
    while(iter->next != NULL) iter = iter->next;

    iter->next = newItem;
    return start;
}

struct listItem* removeFirst(struct listItem* start) {
    if(start == NULL) {
        return NULL;
    }
    struct listItem* newStart = start->next;
    free(start);

    return newStart;
}

// Utility function to do level order traversal
void levelOrder(struct node *root) {
    if (root == NULL)
        return;

    struct listItem *queue = NULL;
    queue = pushBack(queue, root);

    int currentLevel = 1;
    int nextLevel = 0;
    while (queue != NULL) {
        struct node *temp = queue->data;
        printf("%i  ", temp->val);
        queue = removeFirst(queue);
        currentLevel--;
        if (temp->left != NULL) {
            queue = pushBack(queue, temp->left);
            nextLevel++;
        }
        if (temp->right != NULL) {
            queue = pushBack(queue, temp->right);
            nextLevel++;
        }
        if(currentLevel == 0) {
            currentLevel = nextLevel;
            nextLevel = 0;
            printf("\n");
        }
    }
}