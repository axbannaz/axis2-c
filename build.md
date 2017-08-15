install:

libjson-c:  
Dev
    apt install libjson-c-dev libjson-c-doc
RT
    apt install libjson-c[2|3]

apache2
Dev
    apt install apache2-dev
RT
    apt install apache2

libapr
Dev
    apt install libapr1 libapr1-dev libaprutil1-dev
RT
    apt install libapr1

./configure --with-apache2 --with-apr