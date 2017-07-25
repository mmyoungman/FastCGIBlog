#!/bin/bash

g++ -g -DDEBUG=1 -Wno-write-strings -lfcgi -o blog.fcgi blog.cpp