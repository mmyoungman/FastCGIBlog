# What do I need to run the website?

* gcc
* nginx
* fastcgi (sudo apt install libfcgi-dev)
* spawn-fcgi
* apache2-utils (sudo apt install apache2-utils)

# How to run the website

* Put misc/nginx.conf in /etc/nginx
* Generate .htpasswd using apache2-utils (sudo htpasswd -c /etc/nginx/.htpasswd admin)
* sudo systemctl start nginx
* g++ blog.c -o blog.fcgi -lfcgi
* spawn-fcgi -a127.0.0.1 -p9000 -n ./blog.fcgi