main:
	gcc -o main main.c

main2:
	gcc -o main2 main2.c

toExec:
	gcc -o toExec toExec.c

clean:
	rm -f main main2 toExec

all:
	make clean
	make main
	make main2
	make toExec

runAll:
	make all
	./main ignore
	./main handler
	./main mask
	./main pending
	./main2 ignore
	./main2 mask
	./main2 pending
	make clean