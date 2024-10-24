#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include "queue.h"

// structs
typedef struct node *node_t;

typedef struct node {
    char val;
    int bit;
    node_t next;
    node_t prev;
} node;

typedef struct queue {
    node_t front;
    node_t back;
    node_t hand;
    int max_size;
    int curr_size;
} queue;

// constructors & deconstructors
node_t new_node(char val) {
    node_t n = malloc(sizeof(node));
    assert(n != NULL);
    n->val = val;
    n->next = n->prev = NULL;
    n->bit = 0;
    return n;
}

void delete_node(node_t *n) {
    if (n != NULL && *n != NULL) {
        (*n)->next = (*n)->prev = NULL;
        free(*n);
        *n = NULL;
    }
}

queue_t new_queue(int max_size) {
    queue_t q = malloc(sizeof(queue));
    assert(q != NULL);
    q->front = q->back = q->hand = NULL;
    q->max_size = max_size;
    q->curr_size = 0;
    return q;
}

void delete_queue(queue_t *q) {
    if (q != NULL && *q != NULL) {
        node_t n = (*q)->front;
        node_t next = n->next;
        while (n != NULL) {
            delete_node(&n);
            n = next;
            next = n->next;
        }
        (*q)->front = (*q)->back = (*q)->hand = NULL;
        free(*q);
        *q = NULL;
    }
}

// edit functions
void enqueue(queue_t q, char val) {
    node_t n = new_node(val);
    assert(q != NULL);
    if (q->curr_size == 0) {
        q->front = q->back = n;
        q->hand = n;
    }
    if (q->curr_size == 1) {
        q->front->next = n;
        q->front->prev = n;
        n->next = q->front;
        n->prev = q->back;
        q->back = n;
    }
    if (q->curr_size > 1) {
        q->back->next = n;
        q->front->prev = n;
        n->prev = q->back;
        n->next = q->front;
        q->back = n;
    }
    q->curr_size++;
}

void dequeue(queue_t q, char *val) {
    assert(q->curr_size > 0);
    node_t n = q->front;
    if (q->curr_size == 1) {
        q->front = q->back = q->hand = NULL;
    } else {
        n->next->prev = n->prev;
        n->prev->next = n->next;
        q->front = n->next;
        q->back->next = q->front;
    }
    *val = n->val;
    q->curr_size--;
    delete_node(&n);
}

node_t get_node(queue_t q, char val) {
    node_t n;
    if (q->curr_size == 0)
        return NULL;
    for (n = q->front; n->next != q->front && n->next != NULL; n = n->next) {
        if (n->val == val) {
            return n;
        }
    }
    if (n->val == val) {
        return n;
    }
    return NULL;
}

void move_back(queue_t q, char val) {
    node_t n = get_node(q, val);
    assert(n != NULL);
    if (q->curr_size == 1 || n == q->back)
        return;
    if (q->curr_size == 2 || n == q->front) {
        q->back = n;
        q->front = n->next;
        return;
    }
    if (q->curr_size > 2) {
        n->prev->next = n->next;
        n->next->prev = n->prev;
        n->next = q->front;
        n->prev = q->back;
        q->front->prev = n;
        q->back->next = n;
        q->back = n;
        return;
    }
}

void change_val(queue_t q, char val_old, char val_new) {
    node_t n = get_node(q, val_old);
    n->val = val_new;
    q->hand = n->next;
}

void change_bit(queue_t q, char val, int num) {
    assert(num == 0 || num == 1);
    node_t n = get_node(q, val);
    n->bit = num;
}

// access functions
bool contains(queue_t q, char val) {
    if (get_node(q, val) != NULL)
        return true;
    return false;
}

bool full(queue_t q) {
    return q->curr_size == q->max_size;
}

void print_queue(queue_t q) {
    if (q->curr_size < 1) {
        printf("empty\n");
        return;
    }
    node_t n;
    for (n = q->front; n->next != q->front && n->next != NULL; n = n->next) {
        printf("%c %d - ", n->val, n->bit);
    }
    printf("%c %d\n", n->val, n->bit);
}

void get_next_zero(queue_t q, char *val) {
    assert(q->curr_size > 0);
    node_t n = q->hand;
    while (1) {
        if (n->bit == 0) {
            *val = n->val;
            q->hand = n->next;
            return;
        } else {
            n->bit = 0;
            n = n->next;
        }
    }
}
