////////////////////////////////////////////////////////////////////////////////
// All changes in this file are not recognized during testing!
////////////////////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <time.h>
#include <getopt.h>
#include <stdbool.h>

#include "rbtree.h"
#include "utility.h"
#include "cfs.h"

void checkNewNode(struct node *n, int val) {
    if (n == NULL) {
        printf("New node is NULL\n");
    }
    if (n->color != RED || n->val != val) {
        printf("New node is not correctly initialized\n");
    }
}

void checkBSTInvariant(struct node *n) {
    if (n == NULL) {
        return;
    }
    if (n->left != NULL) {
        if (n->left->val <= n->val) {
            checkBSTInvariant(n->left);
        } else {
            printf("BST invariant violated. Left child of node with value %i has bigger value (%i)\n", n->val,
                   n->left->val);
        }
    }
    if (n->right != NULL) {
        if (n->right->val > n->val) {
            checkBSTInvariant(n->right);
        } else {
            printf("BST invariant violated. Right child of node with value %i has smaller/equal value (%i)\n", n->val,
                   n->right->val);
        }
    }
}

void checkRBInvariant1(struct node *n) {
    if (n == NULL) {
        return;
    }
    if (n->color == RED) {
        if ((n->left != NULL && n->right != NULL)) {
            if (n->left->color == RED || n->right->color == RED) {
                printf("RB invariant violated. Child of red node with value %i is also red\n", n->val);
            }
        }
    }
    checkRBInvariant1(n->left);
    checkRBInvariant1(n->right);
}

void checkRBInvariant2(struct node *n) {
    if (n != NULL && n->color != BLACK) {
        printf("RB invariant violated. Root is RED\n");
    }
}

int checkRBInvariant3(struct node *n, int i) {
    if (n == NULL) {
        return i;
    }
    if (n->color == BLACK) {
        ++i;
    }
    int left = checkRBInvariant3(n->left, i);
    int right = checkRBInvariant3(n->right, i);
    if (left != right) {
        printf("RB invariant violated. Child paths of node with value %i doesn't contain same number of black nodes\n",
               n->val);
    }
    return left;
}

int getMaxHeight(struct node *n, int i) {
    if (n == NULL) {
        return i;
    }
    ++i;
    int left = getMaxHeight(n->left, i);
    int right = getMaxHeight(n->right, i);
    return (left < right) ? right : left;
}

void checkRBInvariant4(struct node *n, int size) {
    int height = getMaxHeight(n, 0);
    int maxHeight = (int) ceil(2.0 * log2(size + 1.0));
    if (height > maxHeight) {
        printf("RB invariant violated. Tree has to many levels.\nMax. allowed: %i\nHeight: %i\n", maxHeight, height);
    }
}

// up == True != down
void linearize(int *array, struct node *n, int currentIndex, int max) {
    if (n == NULL) {
        return;
    }
    if (currentIndex >= max) {
        printf("Too many notes. Current node: val: %i\n", n->val);
        return;
    }
    array[currentIndex] = n->val;
    currentIndex++;
    if (n->left != NULL) {
        linearize(array, n->left, currentIndex, max);
    } else {
        if (n->right != NULL) {
            linearize(array, n->right, currentIndex, max);
        } else {
            struct node *temp = n->parent;
            while (true) {
                if (temp == NULL) {
                    // root reached
                    for (int i = currentIndex; i < max; i++) {
                        array[i] = INT_MAX;
                    }
                    return;
                }
                if (temp->right != NULL && temp->right != n) {
                    linearize(array, temp->right, currentIndex, max);
                    break;
                } else {
                    n = temp;
                    temp = temp->parent;
                }
            }
        }
    }
}

int compare(const void *elem1, const void *elem2) {
    return *(int *) elem1 - *(int *) elem2;
}

void checkValues(struct node *root, int start, int end) {
    int *values = malloc((end - start) * sizeof(int));
    CHECK_ALLOC(values);
    linearize(values, root, 0, (end - start));
    qsort(values, end - start, sizeof(int), compare);
    for (int i = start; i < end; ++i) {
        if (values[i] > i) {
            printf("Value %i missing\n", i);
        } else if (values[i] < i) {
            printf("Additional value %i found\n", values[i]);
        }
    }
    free(values);
}

void checkTree(char *prefix, struct node *root, int size) {
    printf("%s: Check leq invariant:\n", prefix);
    checkBSTInvariant(root);
    printf("Done\n");

    printf("%s: Check that direct children of red nodes are black:\n", prefix);
    checkRBInvariant1(root);
    printf("Done\n");

    printf("%s: Check that root is black:\n", prefix);
    checkRBInvariant2(root);
    printf("Done\n");

    printf("%s: Check that each path contains same amount of black nodes:\n", prefix);
    checkRBInvariant3(root, 0);
    printf("Done\n");

    printf("%s: Check max height:\n", prefix);
    checkRBInvariant4(root, size);
    printf("Done\n");
}

void checkRotate0(char *direction, struct node *func(struct node *, struct node *)) {
    printf("Check rotate %s with NULL:\n", direction);
    struct node *node = func(NULL, NULL);
    if (node != NULL) {
        printf("Failed\n");
    }
    printf("Done\n");
}

void checkRotate1(char *direction, struct node *func(struct node *, struct node *)) {
    printf("Check rotate %s with single item:\n", direction);
    struct node *root = malloc(sizeof(struct node));
    CHECK_ALLOC(root);
    root->left = NULL;
    root->right = NULL;
    root->color = BLACK;
    root->val = 2;
    root->parent = (struct node *) 314;
    root = func(root, root);
    if (root->left != NULL) {
        printf("Ptr to left child changed\n");
    }
    if (root->right != NULL) {
        printf("Ptr to right child changed\n");
    }
    if (root->color != BLACK) {
        printf("Color changed\n");
    }
    if (root->parent != (struct node *) 314) {
        printf("Parent changed\n");
    }
    if (root->val != 2) {
        printf("Value changed\n");
    }
    printf("Done\n");
    free(root);
}

void checkRotateLeft2() {
    printf("Check rotate left with two elements:\n");
    struct node *root = malloc(sizeof(struct node));
    CHECK_ALLOC(root);
    struct node *node = malloc(sizeof(struct node));
    CHECK_ALLOC(node);
    struct node *c_root = root;
    struct node *c_node = node;
    root->left = NULL;
    node->left = NULL;
    root->right = node;
    node->right = NULL;
    root->color = BLACK;
    node->color = RED;
    root->val = 2;
    node->val = 3;
    root->parent = NULL;
    node->parent = root;
    root = rotateLeft(root, root);
    if (root == NULL) {
        printf("New root is NULL\n");
    }
    if (root->left == NULL) {
        printf("Ptr to left child incorrect\n");
    } else {
        struct node *left = root->left;
        if (left->left != NULL) {
            printf("Ptr to left child incorrect\n");
        }
        if (left->right != NULL) {
            printf("Ptr to right child incorrect\n");
        }
        if (left->color != BLACK) {
            printf("Node has wrong color\n");
        }
        if (left->parent != root) {
            printf("Node parent incorrect (Should be root)\n");
        }
        if (left->val != 2) {
            printf("Node value incorrect\n");
        }
    }
    if (root->right != NULL) {
        printf("Ptr to right child incorrect\n");
    }
    if (root->color != RED) {
        printf("Root has wrong color\n");
    }
    if (root->parent != NULL) {
        printf("Root parent incorrect\n");
    }
    if (root->val != 3) {
        printf("Root value incorrect\n");
    }
    printf("Done\n");
    free(c_node);
    free(c_root);
}

void checkRotateLeft3() {
    printf("Check rotate left with three elements:\n");
    struct node *root = malloc(sizeof(struct node));
    CHECK_ALLOC(root);
    struct node *node = malloc(sizeof(struct node));
    CHECK_ALLOC(node);
    struct node *node2 = malloc(sizeof(struct node));
    CHECK_ALLOC(node2);
    struct node *c_root = root;
    struct node *c_node = node;
    root->left = NULL;
    node->left = NULL;
    node2->left = NULL;
    root->right = node;
    node->right = node2;
    node2->right = NULL;
    root->color = BLACK;
    node->color = RED;
    node2->color = BLACK;
    root->val = 7;
    node->val = 8;
    node2->val = 9;
    root->parent = NULL;
    node->parent = root;
    node2->parent = node;
    root = rotateLeft(root, root);
    if (root == NULL) {
        printf("New root is NULL\n");
    }
    if (root->left != c_root) {
        printf("Ptr to left child incorrect\n");
    } else {
        struct node *left = root->left;
        if (left->left != NULL) {
            printf("Ptr to left child incorrect\n");
        }
        if (left->right != NULL) {
            printf("Ptr to right child incorrect\n");
        }
        if (left->color != BLACK) {
            printf("Node has wrong color\n");
        }
        if (left->parent != root) {
            printf("Node parent incorrect (Should be root)\n");
        }
        if (left->val != 7) {
            printf("Node value incorrect\n");
        }
        //if(left->)
    }
    if (root->right != node2) {
        printf("Ptr to right child incorrect\n");
    } else {
        struct node *right = root->right;
        if (right->left != NULL) {
            printf("Ptr to left child incorrect\n");
        }
        if (right->right != NULL) {
            printf("Ptr to right child incorrect\n");
        }
        if (right->color != BLACK) {
            printf("Node has wrong color\n");
        }
        if (right->parent != root) {
            printf("Node parent incorrect (Should be root)\n");
        }
        if (right->val != 9) {
            printf("Node value incorrect\n");
        }
    }
    if (root->color != RED) {
        printf("Root has wrong color\n");
    }
    if (root->parent != NULL) {
        printf("Root parent incorrect\n");
    }
    if (root->val != 8) {
        printf("Root value incorrect\n");
    }
    printf("Done\n");
    free(c_node);
    free(c_root);
    free(node2);
}

void checkRotateRight2() {
    printf("Check rotate right with two elements:\n");
    struct node *root = malloc(sizeof(struct node));
    CHECK_ALLOC(root);
    struct node *node = malloc(sizeof(struct node));
    CHECK_ALLOC(node)
    struct node *c_root = root;
    struct node *c_node = node;
    root->left = node;
    node->left = NULL;
    root->right = NULL;
    node->right = NULL;
    root->color = BLACK;
    node->color = RED;
    root->val = 2;
    node->val = 3;
    root->parent = NULL;
    node->parent = root;
    root = rotateRight(root, root);
    if (root == NULL) {
        printf("New root is NULL\n");
    }
    if (root->right == NULL) {
        printf("Ptr to right child incorrect\n");
    } else {
        struct node *right = root->right;
        if (right->left != NULL) {
            printf("Ptr to left child incorrect\n");
        }
        if (right->right != NULL) {
            printf("Ptr to right child incorrect\n");
        }
        if (right->color != BLACK) {
            printf("Node has wrong color\n");
        }
        if (right->parent != root) {
            printf("Node parent incorrect (Should be root)\n");
        }
        if (right->val != 2) {
            printf("Node value incorrect\n");
        }
    }
    if (root->left != NULL) {
        printf("Ptr to left child incorrect\n");
    }
    if (root->color != RED) {
        printf("Root has wrong color\n");
    }
    if (root->parent != NULL) {
        printf("Root parent incorrect\n");
    }
    if (root->val != 3) {
        printf("Root value incorrect\n");
    }
    printf("Done\n");
    free(c_node);
    free(c_root);
}

void checkRotateRight3() {
    printf("Check rotate right with three elements:\n");
    struct node *root = malloc(sizeof(struct node));
    CHECK_ALLOC(root);
    struct node *node = malloc(sizeof(struct node));
    CHECK_ALLOC(node);
    struct node *node2 = malloc(sizeof(struct node));
    CHECK_ALLOC(node2);
    struct node *c_root = root;
    struct node *c_node = node;
    root->left = node;
    node->left = node2;
    node2->left = NULL;
    root->right = NULL;
    node->right = NULL;
    node2->right = NULL;
    root->color = BLACK;
    node->color = RED;
    node2->color = BLACK;
    root->val = 10;
    node->val = 11;
    node2->val = 12;
    root->parent = NULL;
    node->parent = root;
    node2->parent = node;
    root = rotateRight(root, root);
    if (root == NULL) {
        printf("New root is NULL\n");
    }
    if (root->right != c_root) {
        printf("Ptr to right child incorrect\n");
    } else {
        struct node *right = root->right;
        if (right->left != NULL) {
            printf("Ptr to left child incorrect\n");
        }
        if (right->right != NULL) {
            printf("Ptr to right child incorrect\n");
        }
        if (right->color != BLACK) {
            printf("Node has wrong color\n");
        }
        if (right->parent != root) {
            printf("Node parent incorrect (Should be root)\n");
        }
        if (right->val != 10) {
            printf("Node value incorrect\n");
        }
    }
    if (root->left != node2) {
        printf("Ptr to left child incorrect\n");
    } else {
        struct node *left = root->left;
        if (left->left != NULL) {
            printf("Ptr to left child incorrect\n");
        }
        if (left->right != NULL) {
            printf("Ptr to right child incorrect\n");
        }
        if (left->color != BLACK) {
            printf("Node has wrong color\n");
        }
        if (left->parent != root) {
            printf("Node parent incorrect (Should be root)\n");
        }
        if (left->val != 12) {
            printf("Node value incorrect\n");
        }
    }
    if (root->color != RED) {
        printf("Root has wrong color\n");
    }
    if (root->parent != NULL) {
        printf("Root parent incorrect\n");
    }
    if (root->val != 11) {
        printf("Root value incorrect\n");
    }
    printf("Done\n");
    free(c_node);
    free(c_root);
    free(node2);
}

int main(int argc, char **argv) {

    int c;
    char* arg[2];
    arg[0] = argv[0];
    arg[1] = "-nlrsac";
    if(argc == 1) {
        argv = arg;
        argc = 2;
    }
    while ((c = getopt(argc, argv, "nlrsac")) != -1) {
        switch (c) {
            case 'n':
                printf("Check newNode:\n");
                int val = rand();
                struct node *node = newNode(val);
                checkNewNode(node, val);
                free(node);
                printf("Done\n");
                break;
            case 'l':
                checkRotate0("left", rotateLeft);
                checkRotate1("left", rotateLeft);
                checkRotateLeft2();
                checkRotateLeft3();
                break;
            case 'r':
                checkRotate0("right", rotateRight);
                checkRotate1("right", rotateRight);
                checkRotateRight2();
                checkRotateRight3();
                break;
            case 's': {
                struct node *root = NULL;
                for (int i = 0; i < 16; i++) {
                    struct node *node = newNode(i);
                    root = insertRB(root, node);
                }


                checkTree("Static [0..15]", root, 16);

                printf("Static [0..15]: Check contained numbers:\n");
                checkValues(root, 0, 16);
                printf("Done\n");
                iterate_post_order(root, free_node);
                break;
            }
            case 'a': {
                srand(time(NULL));
                int *elements = NULL;
                for (int numElements = 16; numElements <= 65536; numElements <<= 6) {
                    elements = realloc(elements, numElements * sizeof(int));
                    struct node *root = NULL;
                    for (int i = 0; i < numElements; i++) {
                        int randValue = rand();
                        struct node *iter = root;
                        while (iter != NULL && iter->val != randValue) {
                            iter = (randValue <= iter->val) ? iter->left : iter->right;
                        }
                        if (iter != NULL) {
                            --i;
                            continue;
                        }
                        struct node *node = newNode(randValue);
                        root = insertRB(root, node);
                        elements[i] = randValue;
                    }

                    char buffer[64] = {0};
                    sprintf(buffer, "Random %i elem", numElements);
                    checkTree(buffer, root, numElements);


                    int *values = malloc(numElements * sizeof(int));
                    CHECK_ALLOC(values);
                    linearize(values, root, 0, numElements);
                    qsort(values, numElements, sizeof(int), compare);
                    qsort(elements, numElements, sizeof(int), compare);
                    printf("Random %i elem: Check contained numbers:\n", numElements);
                    for (int i = 0; i < numElements; i++) {
                        if (values[i] != elements[i]) {
                            printf("%s Wrong element. Found %i, expected %i\n", buffer, values[i], elements[i]);
                        }
                    }
                    printf("Done\n");
                    free(values);
                    iterate_post_order(root, free_node);
                }
                free(elements);
                break;
            }
            case 'c': {
                struct process *list = NULL;
                struct process *iter = list;
                for (int i = 23; i < 1000; i += 21) {
                    struct process *newItem = malloc(sizeof(struct process));
                    CHECK_ALLOC(newItem);
                    newItem->PID = i;
                    newItem->decay = 2;
                    if (iter == NULL) {
                        list = newItem;
                    } else {
                        iter->next = newItem;
                    }
                    iter = newItem;
                }
                iter->next = NULL;
                printf("Execute CSF:\n");
                struct node *final = cfs(list, 50);
                printf("Done\n\n");
                // check properties of final
                checkTree("CSF final tree", final, 58);

                iterate_post_order(final, free_node);

                while(list != NULL) {
                    struct process *next = list->next;
                    free(list);
                    list = next;
                }
            }
        }
    }

    return 0;
}
