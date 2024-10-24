#pragma once
#include <stdbool.h>

typedef struct queue *queue_t;

// constructor-destructor
queue_t new_queue(int max_size);

void delete_queue(queue_t *q);

// edit functions
void enqueue(queue_t q, char val);

int check_bit(queue_t q, char val);

void dequeue(queue_t q, char *val);

void move_back(queue_t q, char val);

void change_val(queue_t q, char val_old, char val_new);

void change_bit(queue_t q, char val, int num);

// access function
bool contains(queue_t q, char val);

bool full(queue_t q);

void print_queue(queue_t q);

void get_next_zero(queue_t q, char *val);
