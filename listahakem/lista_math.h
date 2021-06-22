#ifndef __LISTA_MATH__
#define __LISTA_MATH__

#include "listat.h"

typedef struct {
  int a;
  int b;
} int2;

typedef struct {
  float a;
  int b;
} floatint;

int intsumma(ilista*, int);
double intmean(ilista*, int);
int2 intmax(ilista* l, int);
int2 intmin(ilista* l, int);
void intjarjesta(ilista*, int2 fun(ilista*, int), int* j, int);
ilista* int_lomitusjarj(ilista* l);
ilista* int_lomitusjarj_pit(ilista*, unsigned);
ilista* intlistalle (ilista*, int, int, int, int);

double floatsumma(flista*, int);
double floatmean(flista*, int);
double floatstd(flista* l, int n);
floatint floatmed(flista* fl, int n);
floatint floatmax(flista* l, int);
floatint floatmin(flista* l, int);
flista* floatjarjesta(flista*, floatint fun(flista*, int), int* j, int);
flista* float_lomitusjarj(flista* l0);
flista* float_lomitusjarj_pit(flista* l0, unsigned n);
flista* floatmovavg(flista* l, int mv, int mo, int pv, int po, int);
float floatavg(flista* l, int mv, int mo, int pv, int po);
flista* floatlistalle(flista* l, int mv, int mo, int pp, int ps);

#endif
