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

void *malloc(size_t size) {
    block_meta *block;
    if (size <= 0) {
        return NULL;
    }

    if (!global_base) { // First call.
        block = request_space(NULL, size);
        if (!block) {
            return NULL;
        }
        global_base = block;
    } else {
        block_meta *last = global_base;
        block = find_free_block(&last, size);
        if (!block) { // Failed to find free block.
            block = request_space(last, size);
            if (!block) {
                return NULL;
            }
        } else { // Found free block
            block->free = 0;
        }
    }
    return(block+1);
}

void free(void *ptr) {
    if (!ptr) {
        return;
    }

    block_meta *block_ptr = (block_meta*)ptr - 1;
    block_ptr->free = 1;
    // TODO: Coalesce free blocks here if possible.
}

int main() {
    int *num = malloc(sizeof(int));
    *num = 4;
    printf("%d\n", *num);
    free(num);
    // Implement more complex allocation and deallocation scenarios here.
    return 0;
}