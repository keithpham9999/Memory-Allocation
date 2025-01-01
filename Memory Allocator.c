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

block_meta *request_space(block_meta* last, size_t size) {
    block_meta *block = sbrk(0);
    void *request = sbrk(size + sizeof(block_meta));
    if (request == (void*) -1) {
        return NULL; // sbrk failed.
    }

    if (last) { // NULL on first request.
        last->next = block;
    }
    block->size = size;
    block->next = NULL;
    block->free = 0;
    return block;
}