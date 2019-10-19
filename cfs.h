#ifndef CFS_H
#define CFS_H

#include "rbtree.h"

struct process {
    int PID;
    int decay;
    struct process* next;
};

/**
 * Execute CFS with given parameters.
 * @param processes List of processes
 * @param time Number of time steps.
 * @return tree after last time step
 */
struct node* cfs(struct process *processes, int time);

#endif // CFS_H
