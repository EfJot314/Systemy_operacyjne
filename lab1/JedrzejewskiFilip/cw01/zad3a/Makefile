
libSta: library.c 
	gcc -Wall -c library.c
	ar cr liblibrary.a library.o

libDyn: library.c 
	gcc -Wall -fPIC $(OPT) -c library.c
	gcc -Wall -shared -fPIC $(OPT) -o liblibrary.so library.o

main: main.c
	gcc -Wall $(OPT) main.c -L. -llibrary

mainDyn: mainDyn.c 
	gcc -Wall $(OPT) mainDyn.c

addToPath:
	export LD_LIBRARY_PATH=/home/filipjedrzejewski/SysOpy/lab1/JedrzejewskiFilip/cw01/zad3a


all:
	make addToPath
	make libSta
	make libDyn 
	make mainDyn
	make main 
	make runTests


clean:
	rm -f a.out liblibrary.so library.o liblibrary.a


simpleTest: 
	touch ./results/results_$(SUF).txt
	./a.out < testCommands > ./results/results_$(SUF).txt


runTests:
	make clean
	make libSta
	make main
	make simpleTest SUF="static"

	make clean
	make libDyn
	make main
	make simpleTest SUF="shared"

	make clean
	make libDyn
	make mainDyn 
	make simpleTest SUF="dynamic"

	make clean






