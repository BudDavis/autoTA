CFLAGS=-Wall -g
all:autoTA chat
chat:chat.o kbhit.o udp3.o
autoTA:autoTA.o kbhit.o udp3.o
clean:
	rm -f *.o autoTA chat
