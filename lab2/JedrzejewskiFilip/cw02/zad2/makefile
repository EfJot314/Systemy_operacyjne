main1:
	gcc main1.c

main2:
	gcc main2.c

manualTest:
	./a.out testowe wyjscie

autoTest:
	./a.out main1.c tmp1 ; ./a.out tmp1 tmp2 ; diff -s tmp2 main1.c

tacTest:
	./a.out main1.c tmp1 ; tac -r -s < main1.c | rev > tmp2 ; diff -sw tmp1 tmp2

make all:
	make clean
	make main1
	make autoTest
	make clean
	make main2
	make autoTest
	make manualTest
	make clean

make clean:
	rm -f a.out tmp1 tmp2