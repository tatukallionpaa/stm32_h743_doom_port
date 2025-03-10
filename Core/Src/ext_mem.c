#include "ext_mem.h"
#include "stdint.h"
#include "stddef.h"


#define EXT_MEM_BEGIN 0xC0000000
#define EXT_MEM_SIZE 0x1F40000
#define EXT_MEM_END EXT_MEM_BEGIN + EXT_MEM_SIZE

static void *ext_mem_top = (void *)EXT_MEM_BEGIN;

void *ext_mem_allocate(uint32_t size)
{
    void *ret_val = NULL;
    if (((uint32_t)ext_mem_top + size) <= EXT_MEM_END)
    {
        ret_val = ext_mem_top;
        ext_mem_top += size;
    }
    return ret_val;
}