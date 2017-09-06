#!/bin/bash

sudo apt update
sudo apt upgrade
sudo apt install nginx libfcgi-dev g++
rm -rf fastcgi-blog
git clone https://github.com/mmyoungman/fastcgi-blog.git
cd fastcgi-blog/code
./build.sh
cd ../build
spawn-fcgi -a127.0.0.1 -p9000 -n ./blog.fcgi