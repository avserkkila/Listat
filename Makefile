listasrc=ylista.c ilista.c strlista.c flista.c
listah=ylista.h ilista.h strlista.h flista.h
listao=ylista.o ilista.o strlista.o flista.o

all: lista_math

listat:
	cd listahakem && \
	gcc -g -c -Wall ${listasrc} && \
	ar rcs liblistat.a ${listao} && \
	rm ${listao}

lista_math:
	cd listahakem && \
	gcc -g -c -Wall lista_math.c ${listasrc} && \
	ar rcs liblista_math.a lista_math.o ${listao} && \
	rm ${listao} lista_math.o

omastring:
	gcc -g -c omastring.c ylista.c strlista.c
	ar rcs libomastring.a omastring.o ylista.o strlista.o
	rm omastring.o ylista.o strlista.o
