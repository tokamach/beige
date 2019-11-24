#pragma once

#include <stddef.h>
#include <stdint.h>

#include "../lisp/env.h"

/*
 * A struct that contains statuses for different parts of the 
 * kernel. Really only one would be instantiated, but I don't
 * really want to make a single static instance for the sake
 * of not polluting the samespace.
 */

typedef enum lisp_state {
    Stopped,
    Running,
    GC
} lisp_state_e;

typedef struct k_status {
    lisp_state_e lisp_state;
    env_t* lisp_world;
    uintptr_t *bitmap_base;
    uintptr_t *arena_base;
} k_status_t;

/*
 * If k_status_enabled = 0, functions won't update it
 * when they're doing stuff. If it's 1, they'll update
 * their respective portion.
 */
uint8_t k_status_enabled;
k_status_t k_status;
