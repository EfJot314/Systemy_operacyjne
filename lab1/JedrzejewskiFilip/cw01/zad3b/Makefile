
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
	export LD_LIBRARY_PATH=/home/filipjedrzejewski/SysOpy/lab1/JedrzejewskiFilip/cw01/zad3b


all:
	make addToPath
	make libSta
	make libDyn 
	make mainDyn
	make main 
	make runTestsWithOptimalization

clean:
	rm -f a.out liblibrary.so library.o liblibrary.a


simpleTest: 
	touch ./results/results_$(SUF).txt
	./a.out < testCommands > ./results/results_$(SUF).txt


runTests:
	make clean
	make libSta OPT=$(OPT)
	make main OPT=$(OPT)
	make simpleTest SUF="static_$(OPT)"

	make clean
	make libDyn OPT=$(OPT)
	make main OPT=$(OPT)
	make simpleTest SUF="shared_$(OPT)"

	make clean
	make libDyn OPT=$(OPT)
	make mainDyn OPT=$(OPT) 
	make simpleTest SUF="dynamic_$(OPT)"

	make clean

runTestsWithOptimalization:
	make runTests OPT="-O0"
	make runTests OPT="-O3"
	make runTests OPT="-Os"





