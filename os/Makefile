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

test_lisp: CC := clang
test_lisp: CFLAGS += -Itests
test_lisp: CFLAGS += -DLISP_TEST
test_lisp: CFLAGS += -g
test_lisp: CFLAGS := $(filter-out -ffreestanding,$(CFLAGS))
test_lisp: INCLD := $(filter-out -Ikernel,$(INCLD))
test_lisp: lisp tests/lisp_test.o
	$(CC) -Itests -Ilisp -o test_lisp tests/lisp_test.o $(LOBJ) -g

os.elf: lisp kernel
	$(CC) -T linker.ld -o os.elf $(CFLAGS) $(KOBJ) $(ASMOBJ) $(LOBJ) -lgcc

os.iso: os.elf
	cp os.bin isodir/boot/
	../toolchain/grub/build/grub-mkrescue -o os.iso isodir

debug: CFLAGS += -DKDEBUG
debug: CFLAGS += -g
debug: os.elf
	../toolchain/gcc/bin/i686-elf-objcopy --only-keep-debug os.elf os.sym

test: kernel $(TESTS)
	./$(TESTS)

clean:
	rm -f kernel/*.o
	rm -f lisp/*.o
	rm -f tests/*.o
	rm -f test_lisp
	rm -f os.elf
	rm -f os.iso
	rm -f os.sym
