#!/bin/bash

export PREFIX="$HOME/os-project/toolchain/"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

brew install bison flex gmp libmpc mpfr libiconv


# binutils
cd $PREFIX/src

if [ ! -f "binutils-2.32.tar.xz" ] ; then
    echo "downloading binutils"
    curl -O "https://ftp.gnu.org/gnu/binutils/binutils-2.32.tar.xz"
fi

if [ -f "binutils-2.32.tar.xz" ] ; then
    echo "untarring binutils"
    tar xzf "binutils-2.32.tar.xz"

    echo "building binutils"

    mkdir build-binutils
    ../binutils-2.32/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
    make
    make install
    
else
    echo "binutils tar not found"
    exit 1
fi

# GCC cross-compiler
cd $PREFIX/src

if [ ! -f "gcc-9.1.0.tar.xz" ] ; then
    echo "downloading gcc tar:"
    curl -O "https://ftp.gnu.org/gnu/gcc/gcc-9.1.0/gcc-9.1.0.tar.xz"
fi

if [ -f "gcc-9.1.0.tar.xz" ] ; then
    echo "untarring gcc"
    tar xzf "gcc-9.1.0.tar.xz"

    echo "building gcc cross compiler"
    mkdir build-gcc
    cd build-gcc
    ../gcc-9.1.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c --without-headers
    make all-gcc
    make all-target-libgcc
    make install-gcc
    make install-target-libgcc
else
    echo "gcc tar not found"
    exit 1
fi
