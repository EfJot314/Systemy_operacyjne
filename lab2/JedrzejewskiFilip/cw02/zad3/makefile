main: main.c
	gcc main.c

all:
	make main
	make toCompare
	make clean

toCompare: a.out
	./a.out
	-wc --bytes *

clean:
	rm -f a.out