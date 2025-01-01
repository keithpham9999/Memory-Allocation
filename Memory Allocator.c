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

