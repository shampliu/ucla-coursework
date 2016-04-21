default:
	gcc -pthread -lmcrypt client.c -o client
	gcc -pthread -lmcrypt server.c -o server

clean:
	rm ./client ./server ./*.txt

dist:
	tar -cvzf lab1b-704291153.tar.gz client.c server.c Makefile README.md my.key 
