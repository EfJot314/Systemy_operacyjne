
libSta: library.c
	gcc -c library.c
	ar cr liblibrary.a library.o

libDyn: library.c 
	gcc -fPIC -c library.c
	gcc -shared -fPIC -o liblibrary.so library.o

main: main.c
	gcc main.c -L. -llibrary

mainDyn: mainDyn.c 
	gcc mainDyn.c

addToPath:
	export LD_LIBRARY_PATH=/home/filipjedrzejewski/SysOpy/lab1

clean:
	rm -f a.out liblibrary.so library.o liblibrary.a



