
#include "heap_allocate.h"

typedef struct
{
    unsigned int size;
    struct block_header_t *next;
    bool free;
} block_header_t;

static block_header_t *heap_head = NULL;
static bool heap_initialized = false;

static void init_heap_allocator(void)
{
    heap_head = (block_header_t *)(HEAP_START_ADDR);
    heap_head->size = MAX_HEAP_SIZE - sizeof(block_header_t);
    heap_head->next = NULL;
    heap_head->free = true;
    heap_initialized = true;
}

void *Heap_Allocate(size_t size)
{
    if (!heap_initialized)
        init_heap_allocator();

    if (heap_head == NULL || size == 0)
        return NULL;

    // Align requested size
    size = (size + HEAP_ALIGNMENT - 1) & ~(HEAP_ALIGNMENT - 1);

    if (size == 0)
        return NULL;

    block_header_t *best = NULL;
    // Best-fit search
    for (block_header_t *chunk = heap_head; chunk != NULL; chunk = chunk->next)
    {
        if (chunk->free && chunk->size >= size)
        {
            if (best == NULL || chunk->size < best->size)
            {
                best = chunk;
            }
        }
    }

    // No suitable block found
    if (best == NULL)
        return NULL;

    // Calculate remaining space after allocation
    size_t excess = best->size - size;

    // Split the block if excess is large enough for a new chunk header + alignment
    if (excess >= sizeof(block_header_t) + HEAP_ALIGNMENT)
    {
        block_header_t *new_block = (block_header_t *)((unsigned char *)best + sizeof(block_header_t) + size);
        new_block->size = excess - sizeof(block_header_t);
        new_block->free = true;
        new_block->next = best->next;

        best->size = size;
        best->next = new_block;
    }

    // Mark block as allocated
    best->free = false;

    // Return pointer past header
    return (unsigned char *)best + sizeof(block_header_t);
}

void Heap_Free(void *ptr)
{
    if (ptr == NULL || heap_head == NULL)
        return;

    // Get the chunk header from the user pointer
    block_header_t *chunk = (block_header_t *)((unsigned char *)ptr - sizeof(block_header_t));
    chunk->free = true;

    // Coalesce with next block if it's free
    if (chunk->next != NULL && chunk->next->free)
    {
        chunk->size += sizeof(block_header_t) + chunk->next->size;
        chunk->next = chunk->next->next;
    }

    // Coalesce with previous block if it's free
    block_header_t *prev = NULL;
    for (block_header_t *curr = heap_head; curr != NULL && curr != chunk; curr = curr->next)
    {
        prev = curr;
    }

    if (prev != NULL && prev->free)
    {
        prev->size += sizeof(block_header_t) + chunk->size;
        prev->next = chunk->next;
    }
}

void *Heap_Calloc(size_t nitems, size_t size)
{
    void *ret_ptr = NULL;
    size_t total_size = nitems * size;
    ret_ptr = Heap_Allocate(total_size);
    if (ret_ptr != NULL)
    {
        memset(ret_ptr, 0, total_size);
    }
    return ret_ptr;
}

void *Heap_Reallocate(void *ptr, size_t size)
{   
    if (ptr == NULL)
    {
        return Heap_Allocate(size);
    }

    if (size == 0)
    {
        Heap_Free(ptr);
        return NULL;
    }

    block_header_t *chunk = (block_header_t *)((unsigned char *)ptr - sizeof(block_header_t));
    if (chunk->size >= size)
    {
        return ptr; // Current block is sufficient
    }

    void *new_ptr = Heap_Allocate(size);
    if (new_ptr != NULL)
    {
        memcpy(new_ptr, ptr, chunk->size);
        Heap_Free(ptr);
    }
    return new_ptr;
}