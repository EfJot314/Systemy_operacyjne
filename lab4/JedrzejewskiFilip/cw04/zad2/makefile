info:
	gcc -o siginfo siginfo.c

resethand:
	gcc -o resethand resethand.c

nodefer:
	gcc -o nodefer nodefer.c

clean:
	rm -f siginfo resethand nodefer

all:
	make clean
	make info
	make resethand
	make nodefer

runAll:
	make all
	./siginfo
	./resethand
	./nodefer
	make clean