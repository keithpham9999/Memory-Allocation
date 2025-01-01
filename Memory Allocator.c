#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct block_meta {
    size_t size;
    struct block_meta *next;
    int free;
} block_meta;

block_meta *global_base = NULL;

block_meta *find_free_block(block_meta **last, size_t size) {
    block_meta *current = global_base;
    while (current && !(current->free && current->size >= size)) {
        *last = current;
        current = current->next;
    }
    return current;
}
