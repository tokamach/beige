#pragma once

#include <stdint.h>

#define IDT_ENTRIES 256

typedef struct idt_entry {
    uint16_t offset_low;
    uint16_t segment_selector;
    uint8_t  zero;
    uint8_t  flags;
    uint16_t offset_high;
}__attribute__((packed)) idt_entry_t;

typedef struct idt_ptr {
    uint16_t limit;
    uintptr_t base;
}__attribute__((packed)) idt_ptr_t;

static struct {
    idt_entry_t entries[IDT_ENTRIES];
    idt_ptr_t pointer;
} idt;

void k_interrupt_init();

//asm link
typedef struct cpu_state {
  uint32_t ds;
  uint32_t edi;
  uint32_t esi;
  uint32_t ebp;
  uint32_t esp;
  uint32_t ebx;
  uint32_t edx;
  uint32_t ecx;
  uint32_t eax;
}__attribute__((packed)) cpu_state_t;

typedef struct stack_state {
    uint32_t error_code;
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
}__attribute__((packed)) stack_state_t;

void interrupt_handler(cpu_state_t cpu, stack_state_t stack, uint32_t interrupt);
