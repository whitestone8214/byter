#!/bin/bash


if (test "$1" = "build"); then
	gcc -O3 -fPIC $(pkg-config --cflags --libs cell) -I. -Wno-int-conversion -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast byter.c -shared -o libbyter.so
elif (test "$1" = "install"); then
	cp -f byter.h /usr/include/ || exit 1
	cp -f libbyter.so /lib/ || exit 1
	cp -f byter.pc /lib/pkgconfig/ || exit 1
fi
