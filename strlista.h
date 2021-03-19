#include "ylista.h"

#ifndef _strlista_h_
typedef struct strlista_s {
  struct strlista_s *edel;
  char* str;
  struct strlista_s *seur;
} strlista;
#endif
#define _strlista_h_

void      _strtulosta(void*);
void      _strtulosta_valein(void*);
void      _strftulosta(FILE*, void*);
void      _strftulosta_valein(FILE*, void*);
char      _strstulostaf(char* s, char* muoto, void* l);
strlista* _strlisaa_kopioiden(void* l, const char* str);
strlista* _strlisaa_kopioiden_taakse(void* l, const char* str);
strlista* _strlistaksi(char* s, const char* erotin);
strlista* _strnouda(void*, int);
strlista* _strkopioi(void*, int n);
strlista* _strpoista(void*, int*, int);
strlista* _strpoista1(void*, char);
strlista* _strpoista_kaikki(void*);
void      _strjarjesta(void*);
void      _strvapautus(void*);
