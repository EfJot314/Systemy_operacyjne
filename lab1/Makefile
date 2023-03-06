
makeLibSta: library.c
	gcc -c library.c
	ar cr liblibrary.a library.o

makeLibDyn: library.c 
	gcc -fPIC -c library.c
	gcc -shared -fPIC -o liblibrary.so library.o
	export LD_LIBRARY_PATH=/home/filipjedrzejewski/SysOpy/lab1

makeMain: main.c
	gcc main.c -L. -llibrary
