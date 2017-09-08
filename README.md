# What do I need to run the website?

* gcc
* nginx
* fastcgi (sudo apt install libfcgi-dev)
* spawn-fcgi
* apache2-utils (sudo apt install apache2-utils)

# How to run the website

* Put misc/nginx.conf in /etc/nginx (You'll need to remove some things from it because it's set up to work on an EC2 instance!)
* Generate .htpasswd using apache2-utils (sudo htpasswd -c /etc/nginx/.htpasswd admin)
* sudo systemctl start nginx
* ./code/build.sh
* ./code/run.sh
* Go to http://localhost:80