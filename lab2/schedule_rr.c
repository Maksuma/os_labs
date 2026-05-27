/**
 * Round-Robin (RR) scheduling algorithm.
 * Each task gets up to QUANTUM time units per round.
 * Tasks with remaining burst are kept in the list for subsequent rounds.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "task.h"
#include "list.h"
#include "cpu.h"
#include "schedulers.h"

static struct node *head = NULL;

void add(char *name, int priority, int burst) {
    Task *task = malloc(sizeof(Task));
    task->name     = name;
    task->tid      = 0;
    task->priority = priority;
    task->burst    = burst;
    insert(&head, task);
}

void schedule() {
    while (head != NULL) {
        struct node *temp = head;
        while (temp != NULL) {
            struct node *next = temp->next;
            Task *task = temp->task;

            int slice = (task->burst > QUANTUM) ? QUANTUM : task->burst;
            run(task, slice);
            task->burst -= slice;

            if (task->burst == 0) {
                delete(&head, task);
                free(task);
            }
            temp = next;
        }
    }
}
