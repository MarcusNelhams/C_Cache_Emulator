#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include "queue.h"

#define FIFO  1
#define LRU   2
#define CLOCK 3

void print_usage() {
    fprintf(stderr, "Usage: ./cacher [-N size] <policy>\n");
}

int main(int argc, char *argv[]) {
    int opt;
    int size = -1;
    int policy = FIFO;

    if (argc > 4) {
        fprintf(stderr, "Invalid number of arguments\n");
        print_usage();
        exit(1);
    }

    while ((opt = getopt(argc, argv, "N:FLC")) != -1) {
        switch (opt) {
        case 'N': size = atoi(optarg); break;
        case 'F': policy = FIFO; break;
        case 'L': policy = LRU; break;
        case 'C': policy = CLOCK; break;
        case '?':
            fprintf(stderr, "Unknown option: %c\n", optopt);
            print_usage();
            exit(1);
            break;
        }
    }

    if (size == -1) {
        fprintf(stderr, "size not set\n");
        exit(1);
    }

    queue_t q = new_queue(size);
    queue_t mem = new_queue(-1);
    char val[3];
    char dq;
    int CO = 0;
    int CA = 0;
    if (policy == FIFO) {
        while (fgets(val, 3, stdin) != NULL) {
            if (contains(q, val[0])) {
                printf("HIT\n");
            } else if (!contains(q, val[0]) && !full(q)) {
                printf("MISS\n");
                enqueue(q, val[0]);
                CO++;
            } else {
                printf("MISS\n");
                dequeue(q, &dq);
                if (!contains(mem, dq))
                    enqueue(mem, dq);
                enqueue(q, val[0]);
                if (contains(mem, val[0]))
                    CA++;
                else
                    CO++;
            }
        }
        printf("%d %d\n", CO, CA);
    } else if (policy == LRU) {
        while (fgets(val, 3, stdin) != NULL) {
            if (contains(q, val[0])) {
                printf("HIT\n");
                move_back(q, val[0]);
            } else if (!contains(q, val[0]) && !full(q)) {
                printf("MISS\n");
                enqueue(q, val[0]);
                CO++;
            } else {
                printf("MISS\n");
                dequeue(q, &dq);
                if (!contains(mem, dq))
                    enqueue(mem, dq);
                enqueue(q, val[0]);
                if (contains(mem, val[0]))
                    CA++;
                else
                    CO++;
            }
        }
        printf("%d %d\n", CO, CA);
    } else {
        while (fgets(val, 3, stdin) != NULL) {
            //printf("val: %c\n", val[0]);
            if (contains(q, val[0])) {
                printf("HIT\n");
                change_bit(q, val[0], 1);
            } else if (!contains(q, val[0]) && !full(q)) {
                printf("MISS\n");
                enqueue(q, val[0]);
                CO++;
            } else {
                printf("MISS\n");
                get_next_zero(q, &dq);
                if (!contains(mem, dq))
                    enqueue(mem, dq);
                change_val(q, dq, val[0]);
                if (contains(mem, val[0]))
                    CA++;
                else
                    CO++;
            }
            //print_queue(q);
            //printf("\n");
        }
        printf("%d %d\n", CO, CA);
    }
    return 0;
}
