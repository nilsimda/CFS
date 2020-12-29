#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "rbtree.h"

struct node *newNode (int val){
    struct node node = {val, RED, NULL, NULL, NULL};
    return &node;
}