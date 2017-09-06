#!/bin/bash
debug="-g -DDEBUG=1"
flags="-Wno-write-strings -fpermissive"

cd ..
mkdir -p build
cd build

g++ $debug $flags  -o blog.fcgi ../code/main.cpp -lfcgi
#g++ $debug $flags -o test ../code/test.cpp -lfcgi