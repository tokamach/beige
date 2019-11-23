#!/usr/bin/env bash

export PREFIX="$HOME/os-project/toolchain/gdb"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

if [ ! -f "$PREFIX/src" ] ; then
    mkdir -p $PREFIX/src
fi

cd $PREFIX/src

if [ ! -f "gdb-8.3.tar.xz" ] ; then
    echo "Downloading GDB"
    curl -O "https://ftp.gnu.org/gnu/gdb/gdb-8.3.tar.xz"
fi

if [ -f "gdb-8.3.tar.xz" ] ; then
    echo "untarring gdb"
    tar xzf "gdb-8.3.tar.xz"

    echo "building gdb"
    mkdir build-gdb
    cd build-gdb

    ../gdb-8.3/configure --prefix=$PREFIX --target=$TARGET
    make
    make install
else
    echo "gdb tar not found"
    exit 1
fi
