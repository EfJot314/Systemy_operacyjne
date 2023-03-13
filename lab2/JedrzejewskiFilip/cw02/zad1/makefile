libVer:
	gcc mainlib.c

sysVer:
	gcc mainsys.c

all:
	make libVer
	make compare
	make clean
	make sysVer
	make compare
	make clean

compare:
	./a.out i x mainlib.c wyjscie ; tr i x < mainlib.c > tmp ; diff -s tmp wyjscie

clean:
	rm -f a.out