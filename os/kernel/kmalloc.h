#pragma once

#include <stdint.h>
#include <stddef.h>

#include "multiboot.h"

typedef enum bs {
    Free,
    Used
} BitmapStatus;

void  k_malloc_init(multiboot_info_t* mbd);
void* kmalloc(size_t size);
void  kfree(void* block);
