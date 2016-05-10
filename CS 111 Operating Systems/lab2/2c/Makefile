default:
	gcc -pthread lab2c.c -o lab2c

gprof:
	gcc -pg -pthread lab2c.c -o lab2c

clean:
	rm lab2c

dist: 
	tar -cvzf lab2c-704291153.tar.gz lab2c.c Makefile README.md SortedList.c SortedList.h thread-to-list-vs-cost.png
