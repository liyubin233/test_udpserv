CC = gcc
CFLAGS = -Ilib -g -O2 -D_REENTRANT -Wall
LIBS = ./libunp.a -lpthread

PROGS = udpserv01 udpcli01 udpcliloop udpservloop tcp_udp_serv

all: ${PROGS}

udpserv01: udpserv01.o
	$(CC) ${CFLAGS} -o $@ udpserv01.o ${LIBS}
udpcli01: udpcli01.o
	$(CC) ${CFLAGS} -o $@ udpcli01.o ${LIBS}
udpcliloop: udpcliloop.o
	$(CC) ${CFLAGS} -o $@ udpcliloop.o ${LIBS}
udpservloop: udpservloop.o
	$(CC) ${CFLAGS} -o $@ udpservloop.o ${LIBS}
tcp_udp_serv: tcp_udp_serv.o
	$(CC) ${CFLAGS} -o $@ tcp_udp_serv.o ${LIBS}

clean:
	rm -rf  *.o *.lc
clean_app:
	rm -rf $(PROGS)
