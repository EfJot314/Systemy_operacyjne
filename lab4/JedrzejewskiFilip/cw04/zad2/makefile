info:
	gcc -o siginfo siginfo.c

resethand:
	gcc -o resethand resethand.c

clean:
	rm -f siginfo resethand

all:
	make clean
	make info
	make resethand

runAll:
	make all
	./siginfo
	./resethand
	make clean