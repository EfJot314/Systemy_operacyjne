main:
	gcc main.c

compare:
	./a.out ./
	-find ./ | xargs wc --bytes


all:
	make main
	make compare
	make clean


clean:
	rm -f a.out
