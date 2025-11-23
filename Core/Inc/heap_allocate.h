
#ifndef __HEAP_ALLOCATE_H
#define __HEAP_ALLOCATE_H

#include <stdbool.h>

#define HEAP_START_ADDR 0xC0000000
#define MAX_HEAP_SIZE 0x2000000
#define HEAP_ALIGNMENT 8


typedef struct
{
    unsigned int size;
    struct block_header_t *next;
    bool free;
} block_header_t;

#endif //__HEAP_ALLOCATE_H