CC     := ../toolchain/gcc/bin/i686-elf-gcc
AS     := ../toolchain/gcc/bin/i686-elf-as
CFLAGS := -ffreestanding -nostdlib -Wall -Wextra -mgeneral-regs-only
INCLD  := -Ikernel -Ilisp
KOBJ   := $(shell find kernel -name '*.c' | sed -e "s/\\.c.*/\\.c.o/g")
ASMOBJ := $(shell find kernel -name '*.s' | sed -e "s/\\.s.*/\\.s.o/g")
LOBJ   := $(shell find lisp -name '*.c' | sed -e "s/\\.c.*/\\.c.o/g")

all: os.bin

%.s.o: %.s
	$(AS) $< -o $@

%.c.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS) $(INCLD)

kernel: $(KOBJ) $(shell find kernel -name '*.s' | sed -e "s/\\.s.*/\\.s.o/g")

lisp: $(LOBJ)

lisp-test: CC := gcc-9
lisp-test: CFLAGS += -Itests
lisp-test: CFLAGS += -DLISP_TEST
lisp-test: CFLAGS += -g
lisp-test: CFLAGS := $(filter-out -ffreestanding,$(CFLAGS))
lisp-test: INCLD  := $(filter-out -Ikernel,$(INCLD))
lisp-test: LOBJ   := $(filter-out lisp/kernel_env.c.o,$(LOBJ))
lisp-test: lisp tests/lisp_test.o
	echo $(LOBJ)
	$(CC) -Itests -Ilisp -o lisp_test tests/lisp_test.o $(LOBJ) -g

os.elf: lisp kernel
	$(CC) -T linker.ld -o os.elf $(CFLAGS) $(KOBJ) $(ASMOBJ) $(LOBJ) -lgcc

os.iso: os.elf
	cp os.elf isodir/boot/
	../toolchain/grub/build/grub-mkrescue -o os.iso isodir

#debug: CFLAGS += -DKDEBUG
debug: CFLAGS += -g
debug: os.elf
	../toolchain/gcc/bin/i686-elf-objcopy --only-keep-debug os.elf os.sym

test: kernel $(TESTS)
	./$(TESTS)

clean:
	rm -f kernel/*.o
	rm -f lisp/*.o
	rm -f tests/*.o
	rm -f lisp_test
	rm -f os.elf
	rm -f os.iso
	rm -f os.sym
