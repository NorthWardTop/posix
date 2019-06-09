.PHONY:all
all:client server

client:client.c msgqueue.c
	gcc -w -o client client.c msgqueue.c
server:server.c msgqueue.c
	gcc -w -o server server.c msgqueue.c

.PHONY:clean
clean:
	rm -rf server client
