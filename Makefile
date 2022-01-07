all: server client


server: server.o callback.o list.o
	gcc server.o callback.o list.o -o server -levent

server.o: server.c
	gcc -c server.c

callback.o: callback.c
	gcc -c callback.c

list.o: list.c
	gcc -c list.c

client: client.o
	gcc client.o -o client

client.o: client.c
	gcc -c client.c

clean:
	rm -rf *.o server client
