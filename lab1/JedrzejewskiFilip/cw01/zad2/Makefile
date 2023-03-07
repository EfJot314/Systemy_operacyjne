
libSta: library.c 
	gcc -Wall -c library.c
	ar cr liblibrary.a library.o

libDyn: library.c 
	gcc -Wall -fPIC -c library.c
	gcc -Wall -shared -fPIC -o liblibrary.so library.o

main: main.c
	gcc -Wall main.c -L. -llibrary

mainDyn: mainDyn.c 
	gcc -Wall mainDyn.c

addToPath:
	export LD_LIBRARY_PATH=/home/filipjedrzejewski/SysOpy/lab1/JedrzejewskiFilip/cw01/zad2


all:
	make addToPath
	make libSta
	make libDyn
	make mainDyn
	make main 

clean:
	rm -f a.out liblibrary.so library.o liblibrary.a







