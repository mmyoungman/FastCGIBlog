# What do I need to run the FCGI app?

* gcc
* nginx
* fastcgi (sudo apt install libfcgi-dev) // Or maybe you don't need this?

# How to run the FCGI app

* Copy nginx.conf to /etc/nginx 
* sudo systemctl start nginx
* ./code/build.sh
* ./build/server.fcgi
* Go to http://localhost:80
* Observe server.fcgi parse the incoming info from nginx
