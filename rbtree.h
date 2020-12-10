#ifndef RBTREE_H
#define RBTREE_H

#include <stdbool.h>

/**
 * Enumeration representing the two colors.
 */
enum Color {
    RED, BLACK
};

/**
 * Red-Black-Tree node.
 */
struct node {
    int val;    // the tree is sorted according to this value
    bool color;
    void *data;
    struct node *left;
    struct node *right;
    struct node *parent;
};

/**
 * Generate a new node and initalize all pointers to NULL, the attribute val to parameter val and the color to RED
 * @param val
 * @return
 */
struct node *newNode(int val);

/**
 * Perform a left rotation
 * @param root
 * @param n
 * @return
 */
struct node *rotateLeft(struct node *root, struct node *n);

/**
 * Perform a right rotation
 * @param root
 * @param n
 * @return
 */
struct node *rotateRight(struct node *root, struct node *n);

/**
 * Insert newNode into tree.
 * @param root
 * @param newNode
 */
struct node *insertRB(struct node *root, struct node *newNode);

#endif // RBTREE_H