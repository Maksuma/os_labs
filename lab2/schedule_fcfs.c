/**
 * First-Come First-Served (FCFS) scheduling algorithm.
 * Tasks are run in the order they appear in the list.
 * Since all tasks arrive simultaneously, the list order is valid for FCFS.
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
    task->name  = name;
    task->tid   = 0;
    task->priority = priority;
    task->burst = burst;
    insert(&head, task);
}

void schedule() {
    struct node *temp = head;
    while (temp != NULL) {
        struct node *next = temp->next;
        run(temp->task, temp->task->burst);
        delete(&head, temp->task);
        free(temp->task);
        temp = next;
    }
}
