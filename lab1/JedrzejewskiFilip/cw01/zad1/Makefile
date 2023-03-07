
libSta: library.c 
	gcc -Wall -c library.c
	ar cr liblibrary.a library.o

libDyn: library.c 
	gcc -Wall -fPIC -c library.c
	gcc -Wall -shared -fPIC -o liblibrary.so library.o

all:
	make libSta
	make libDyn

clean:
	rm -f liblibrary.so library.o liblibrary.a



