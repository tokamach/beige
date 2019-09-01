#pragma once

#include <stdint.h>
#include <stddef.h>

#include "multiboot.h"

typedef struct HeapHeader {
    uint8_t marked;
    size_t size;
} HeapHeader_t;

void k_malloc_init(multiboot_info_t* mbd);

void* kmalloc(size_t size);
void  kfree(void* block);

void k_mem_print_blocks();
