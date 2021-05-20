listasrc=ylista.c ilista.c strlista.c flista.c
listao=ylista.o ilista.o strlista.o flista.o

all: lista_math

listat:
	gcc -g -c -Wall ${listasrc}
	ar rcs liblistat.a ${listao}

lista_math:
	gcc -g -c -Wall lista_math.c ${listasrc}
	ar rcs liblista_math.a lista_math.o ${listao}
	rm ${listao} lista_math.o
