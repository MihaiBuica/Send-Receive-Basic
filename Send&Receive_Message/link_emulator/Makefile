all: link lib.o

link: link.o queue.o
	gcc -pthread -g link.o queue.o -o link -O3 

.c.o: 
	gcc -Wall -g -c $? -O3

clean:
	rm -f *.o link
