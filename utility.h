#ifndef UTILITY_H
#define UTILITY_H

#include "rbtree.h"

/**
 * Print value of node.
 * @param node
 */
void print_val(struct node *node);

/**
 * Free memory allocated by node.
 * @param node
 */
void free_node(struct node *node);

/**
 * Iterate in in-order and apply f to each node.
 * @param node
 * @param f function
 */
void iterate_in_order(struct node *node,
                      void (*f)(struct node *n));

/**
 * Iterate in post-order and apply f to each node.
 * @param node
 * @param f function
 */
void iterate_post_order(struct node *node,
                        void (*f)(struct node *n));

/**
 * Print content of tree level-wise.
 * @param root
 */
void levelOrder(struct node *root);

#define CHECK_ALLOC(ptr) {                              \
    if (ptr == NULL) {                                  \
        printf("Memory allocation failed. Abort...\n"); \
        exit(1);                                        \
    }                                                   \
    }

#endif // UTILITY_H