CC     := ../toolchain/bin/i686-elf-gcc
AS     := ../toolchain/bin/i686-elf-as
CFLAGS := -ffreestanding -Wall -Wextra -Ikernel -Ilisp
KOBJ   := $(shell find kernel -name '*.o')
LOBJ   := $(shell find lisp -name '*.o')

all: os.bin

boot.o:
	$(AS) kernel/boot.s -o kernel/boot.o

tests/lisp_test.o: tests/lisp_test.c
	$(CC) -c -o $@ $< -Ilisp -Itests

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

kernel: boot.o $(shell find kernel -name '*.c' | sed -e "s/\\.c.*/\\.o/g")

lisp: $(shell find lisp -name '*.c' | sed -e "s/\\.c.*/\\.o/g")

test_lisp: CC := clang
test_lisp: CFLAGS += -Itests
test_lisp: CFLAGS += -DLISP_TEST
test_lisp: CFLAGS := $(filter-out -ffreestanding,$(CFLAGS))
test_lisp: CFLAGS := $(filter-out -Ikernel,$(CFLAGS))
test_lisp: lisp tests/lisp_test.o
	$(CC) -Itests -Ilisp -o test_lisp tests/lisp_test.o $(LOBJ)

os.bin: lisp kernel
	$(CC) -T linker.ld -o os.bin -ffreestanding -nostdlib $(KOBJ) $(LOBJ) -lgcc

debug: CFLAGS += -DDEBUG
debug: os.bin

test: kernel $(TESTS)
	./$(TESTS)

clean:
	rm -f kernel/*.o
	rm -f lisp/*.o
	rm -f os.bin
