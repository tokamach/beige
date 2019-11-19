#!/usr/bin/env bash

cp os.bin isodir/boot/
../grub/build/grub-mkrescue -o os.iso isodir

#mkdir tmp
#../grub/build/grub-mkimage -p /boot -o tmp/os.img multiboot sh fat
