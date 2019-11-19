#!/usr/bin/env bash

export PREFIX="$HOME/os-project/toolchain"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"
# for gettext
export PATH="/usr/local/opt/gettext/bin:$PATH"

git clone git://git.savannah.gnu.org/grub.git
git clone https://github.com/vertis/objconv.git

cd objconv
g++ -o objconv -O2 src/*.cpp
cd ..

export PATH="$HOME/os-project/grub/objconv:$PATH"

cd grub
./bootstrap
./autogen.sh
cd ..
mkdir build
cd build
../grub/configure --disable-werror TARGET_CC=i686-elf-gcc TARGET_OBJCOPY=i686-elf-objcopy TARGET_STRIP=i686-elf-strip TARGET_NM=i686-elf-nm TARGET_RANLIB=i686-elf-ranlib --target=i686-elf
make

