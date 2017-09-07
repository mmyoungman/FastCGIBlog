#!/bin/bash

# NOTE: Add "/home/ubuntu/fastcgi-blog/misc/ec2-startup.sh" line to /etc/rc.local on ec2 ubuntu instance so that fcgi app automatically runs each time ec2 instance is restarted

sudo apt update
sudo apt upgrade
sudo apt install nginx libfcgi-dev g++
cd /home/ubuntu
rm -rf fastcgi-blog
git clone https://github.com/mmyoungman/fastcgi-blog.git
sudo cp fastcgi-blog/misc/nginx.conf /etc/nginx/
#sudo cp fastcgi-blog/misc/.htpasswd /etc/nginx/
sudo systemctl restart nginx
cd fastcgi-blog/code
./build.sh
cd ../build
spawn-fcgi -a127.0.0.1 -p9000 -n ./blog.fcgi