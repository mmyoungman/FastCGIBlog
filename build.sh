#!/bin/bash
debug="-g -DDEBUG=1"
flags="-Wno-write-strings"
g++ $debug $flags -lfcgi -o blog.fcgi blog.cpp