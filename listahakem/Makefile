listasrc=ylista.c ilista.c strlista.c flista.c
listao=ylista.o ilista.o strlista.o flista.o

all: lista_math

listat:
	gcc -g -c -Wall ${listasrc}
	ar rcs liblistat.a ${listao}
	rm ${listao}
	mv liblistat.a ../kirjastot

lista_math:
	gcc -g -c -Wall lista_math.c ${listasrc} -lm
	ar rcs liblista_math.a lista_math.o ${listao}
	rm ${listao} lista_math.o
	mv liblista_math.a ../kirjastot
