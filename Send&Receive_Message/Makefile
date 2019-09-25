all: send recv

link_emulator/lib.o:
	$(MAKE) -C link_emulator

send: send.o link_emulator/lib.o add_struct.o
	gcc -g send.o link_emulator/lib.o add_struct.o add_struct.h -o send

recv: recv.o link_emulator/lib.o
	gcc -g recv.o link_emulator/lib.o add_struct.o add_struct.h -o recv

#.c.o:
#	gcc -Wall -g -c $?
send.o: send.c
	gcc -Wall -g -c send.c -o send.o
recv.o: recv.c
	gcc -Wall -g -c recv.c -o recv.o
add_struct.o: add_struct.c
	gcc -Wall -g -c add_struct.c -o add_struct.o	
clean:
	$(MAKE) -C link_emulator clean
	-rm -f *.o send recv
