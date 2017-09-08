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

# How to set up on AWS EC2

* Create an EC2 instance
* Create key pair to ssh into instance
* Use Elastic IP to get permanent IP for server
* Create security group so that you can access ec2 instance (80/22/443)
* Use Route53 to create A records for www.domain.com and domain.com
* git clone fastcgi-blog repository via ssh
* Use certbot to get HTTPS working: https://certbot.eff.org/#ubuntuxenial-nginx
* Will need to edit misc/nginx.conf and misc/ec2-startup.sh