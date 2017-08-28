#!/bin/bash
debug="-g -DDEBUG=1"
flags="-Wno-write-strings -fpermissive"

cd ..
mkdir -p build
cd build

g++ $debug $flags -lfcgi -o blog.fcgi ../code/main.cpp
#g++ $debug $flags -lfcgi -o test ../code/test.cpp