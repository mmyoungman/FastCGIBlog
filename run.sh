#!/bin/bash

#./blog.fcgi
spawn-fcgi -a127.0.0.1 -p9000 -n ./blog.fcgi 
