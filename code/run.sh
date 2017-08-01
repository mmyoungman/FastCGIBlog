#!/bin/bash

cd ../build

spawn-fcgi -a127.0.0.1 -p9000 -n ./blog.fcgi 
#./test