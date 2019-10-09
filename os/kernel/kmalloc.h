#pragma once

#include <stdint.h>
#include <stddef.h>

#include "multiboot.h"

typedef enum bs {
    Free,
    Used
} bitmap_status;

void  k_malloc_init(multiboot_info_t* mbd);
void* kmalloc(size_t size);
void  kfree(void* block);
