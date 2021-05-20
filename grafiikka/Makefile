incdir=-I/usr/include/SDL2 -I../listahakem
libdir=-L../grafiikka
libs=-lSDL2 -lSDL2_ttf -llistat

tekstigrafiikka: tekstigraf.c tekstigraf.h
	gcc -g -c -Wall tekstigraf.c ${incdir} ${libdir} ${libs}
	ar rcs libtekstigraf.a tekstigraf.o
	rm tekstigraf.o
