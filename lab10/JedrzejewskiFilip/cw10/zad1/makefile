server:
	gcc -o server server.c -lpthread

client:
	gcc -o client client.c -lpthread

clean:
	rm -f server client

all:
	make clean
	make server
	make client