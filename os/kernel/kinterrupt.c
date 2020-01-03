#include "kinterrupt.h"

#include <stdint.h>

#include "kmalloc.h"
#include "kterm.h"
#include "kpic.h"
#include "kgdt.h"

/* Defined in interrupt.s */
void interrupt_handler_0();
void interrupt_handler_1();
void interrupt_handler_2();
void interrupt_handler_3();
void interrupt_handler_4();
void interrupt_handler_5();
void interrupt_handler_6();
void interrupt_handler_7();
void interrupt_handler_8();
void interrupt_handler_9();
void interrupt_handler_10();
void interrupt_handler_11();
void interrupt_handler_12();
void interrupt_handler_13();
void interrupt_handler_14();
void interrupt_handler_15();
void interrupt_handler_16();
void interrupt_handler_17();
void interrupt_handler_18();
void interrupt_handler_19();

void interrupt_handler_32();
void interrupt_handler_33();
void interrupt_handler_34();
void interrupt_handler_35();
void interrupt_handler_36();
void interrupt_handler_37();
void interrupt_handler_38();
void interrupt_handler_39();
void interrupt_handler_40();
void interrupt_handler_41();
void interrupt_handler_42();
void interrupt_handler_43();
void interrupt_handler_44();
void interrupt_handler_45();
void interrupt_handler_46();
void interrupt_handler_47();

void k_add_interrupt(uint8_t  entry,
		     uint32_t offset,
		     uint8_t type,
		     uint8_t  dpl,
		     uint16_t size)
{
    uint8_t flags = 0;
    flags |= (1 << 7); //is present, set to 0 for unused interrupt
    flags |= ((dpl & 0b11) << 5); //DPL, descriptor privelege level (0-3)
    flags |= (size << 4); //size (set to 0 for interrupt and trap gates)
    flags |= type; //gate type
    
    idt.entries[entry].offset_low       = (uint16_t)offset & 0xffff;
    idt.entries[entry].offset_high      = (offset >> 16)   & 0xffff;

    idt.entries[entry].segment_selector = 0x08; //TODO: chuck in a constant
    idt.entries[entry].zero             = 0;
    idt.entries[entry].flags            = 0x8e; // | 0x60; //flags;
}

#define IDT_ADD_GATE(n) \
    k_add_interrupt(n, (uintptr_t)&interrupt_handler_##n, 1, 0, 0);

void k_interrupt_init()
{
    idt_ptr_t* idt_ptr = &idt.pointer;
    idt_ptr->limit = IDT_ENTRIES - 1;
    idt_ptr->base  = (uint32_t)&idt.entries[0];

    for(uint8_t i = 0; i < IDT_ENTRIES - 1; i++)
	idt.entries[i] = (idt_entry_t){0, 0, 0, 0, 0};

    /* Exceptions */
    IDT_ADD_GATE(0);
    IDT_ADD_GATE(1);
    IDT_ADD_GATE(2);
    IDT_ADD_GATE(3);
    IDT_ADD_GATE(4);
    IDT_ADD_GATE(5);
    IDT_ADD_GATE(6);
    IDT_ADD_GATE(7);
    IDT_ADD_GATE(8);
    IDT_ADD_GATE(9);
    IDT_ADD_GATE(10);
    IDT_ADD_GATE(11);
    IDT_ADD_GATE(12);
    IDT_ADD_GATE(13);
    IDT_ADD_GATE(14);
    IDT_ADD_GATE(15);
    IDT_ADD_GATE(16);
    IDT_ADD_GATE(17);
    IDT_ADD_GATE(18);
    IDT_ADD_GATE(19);

    /* IRQs */
    IDT_ADD_GATE(32);
    IDT_ADD_GATE(33);
    IDT_ADD_GATE(34);
    IDT_ADD_GATE(35);
    IDT_ADD_GATE(36);
    IDT_ADD_GATE(37);
    IDT_ADD_GATE(38);
    IDT_ADD_GATE(39);
    IDT_ADD_GATE(40);
    IDT_ADD_GATE(41);
    IDT_ADD_GATE(42);
    IDT_ADD_GATE(43);
    IDT_ADD_GATE(44);
    IDT_ADD_GATE(45);
    IDT_ADD_GATE(46);
    IDT_ADD_GATE(47);
    
    
    //load IDT
    load_idt(idt_ptr);
    
    enable_interrupts();
    
    k_println("interrupts enabled");
}

 
__attribute__((interrupt))
void interrupt_handler(interrupt_frame_t* frame)
{
    k_print("Caught Interrupt: ");
    k_print_hex(frame->error_code);
    k_print("\n");
    //asm("hlt");
    
    // call exception or software dependent on error
    
    k_pic_eoi(frame->error_code);
}

//TODO: write exception handler (error code in args)
//TODO: write software interrupt (syscall) handler
