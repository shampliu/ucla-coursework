1. login 
2. create ami, select 32 bit, choose ubuntu 14.10
3. launch instance
4. create key pair, type "key"
5. save as key.pem
6. login to your seas account
7. ssh -i <path_to>/key.pem ubuntu@<public ip found on view instance tab>

(on ec2)
8. sudo apt-get update
9. sudo apt-get install gcc
10. sudo apt-get install make
11. wget <server tar ball url>
12. untar apply patch
13. ... regular stuff

