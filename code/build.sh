#!/bin/bash
debug="-g -DDEBUG"
flags="-Wno-write-strings -fpermissive"

cd ..
mkdir -p build
cd build

g++ $debug $flags -o server.fcgi ../code/fastcgi-server.c
