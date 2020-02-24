#pragma once

#include <stdint.h>

#define IDT_ENTRIES 256
#define NUM_OF_INT_HANDLERS IDT_ENTRIES

typedef struct idt_entry {
    uint16_t offset_low;
    uint16_t segment_selector;
    uint8_t  zero;
    uint8_t  flags;
    uint16_t offset_high;
}__attribute__((packed)) idt_entry_t;

typedef struct idt_ptr {
    uint16_t limit;
    uint32_t base;
}__attribute__((packed)) idt_ptr_t;

static struct {
    idt_entry_t entries[IDT_ENTRIES];
    idt_ptr_t pointer;
} idt;

// New interrupt data structures
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
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
    uint32_t user_esp;
    uint32_t user_ss;
}__attribute__((packed)) stack_state_t;

typedef struct idt_info {
    uint32_t idt_index;
    uint32_t err_code;
}__attribute__((packed)) idt_info_t;

typedef void (*interrupt_handler_t)(cpu_state_t cpu_state, idt_info_t idt_info, stack_state_t stack_state);


void k_interrupt_init();
void k_register_interrupt_handler(uint32_t index, interrupt_handler_t handler);
void forward_interrupt(cpu_state_t cpu_state, idt_info_t idt_info, stack_state_t stack_state);

//defined in kinterrupt.s
void load_idt(idt_ptr_t *idt_addr);
void enable_interrupts();
void disable_interrupts();
