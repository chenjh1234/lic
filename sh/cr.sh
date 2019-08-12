openssl genrsa -out cakey.pem 1024
openssl req -new -x509 -day 3650 -key cakey.pem -out cacert.pem
