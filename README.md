# What do I need to run the website?

* gcc
* nginx
* fastcgi (sudo apt install libfcgi-dev)
* spawn-fcgi

# How to run the website

* Put misc/nginx.conf in /etc/nginx
* sudo systemctl start nginx
* g++ blog.c -o blog.fcgi -lfcgi
* spawn-fcgi -a127.0.0.1 -p9000 -n ./blog.fcgi