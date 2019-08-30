CC     := ../toolchain/bin/i686-elf-gcc
AS     := ../toolchain/bin/i686-elf-as
CFLAGS := -ffreestanding -Wall -Wextra
#DEPS   := kernel/kmem.h kernel/kstd.h
KOBJ   := kernel/boot.o kernel/kmain.o kernel/kstd.o kernel/kterm.o kernel/kmem.o
TESTS  := tests/atoi.o

all: os.bin

boot.o:
	$(AS) kernel/boot.s -o kernel/boot.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

kernel: $(KOBJ)

os.bin: kernel
	$(CC) -T linker.ld -o os.bin -ffreestanding -O2 -nostdlib $(KOBJ) -lgcc

debug: CFLAGS += -DDEBUG
debug: os.bin

test: kernel $(TESTS)
	./$(TESTS)

clean:
	rm kernel/*.o
