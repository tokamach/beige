CC     := ../toolchain/bin/i686-elf-gcc
AS     := ../toolchain/bin/i686-elf-as
CFLAGS := -ffreestanding -O2 -Wall -Wextra

all: kernel

boot.o:
	$(AS) kernel/boot.s -o kernel/boot.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

kernel: boot.o kmain.o kterm.o
	$(CC) -T linker.ld -o os.bin -ffreestanding -O2 -nostdlib kernel/boot.o kernel/kmain.o kernel/kterm.o -lgcc

clean:
	rm kernel/*.o
