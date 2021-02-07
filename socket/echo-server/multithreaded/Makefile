.PHONY: all server client

all: server client

server:
	gcc -o $@ server.c -pthread

client:
	gcc -o $@ client.c

clean:
	rm -rf server client
