#include "ylista.h"

#ifndef _strlista_h_
typedef struct strlista_s {
  struct strlista_s *edel;
  char* str;
  struct strlista_s *seur;
} strlista;
#endif
#define _strlista_h_

void      _strtulosta(strlista*);
void      _strtulosta_valein(strlista*);
void      _strftulosta(FILE*, strlista*);
void      _strftulosta_valein(FILE*, strlista*);
char      _strstulostaf(char* s, char* muoto, strlista* l);
strlista* _strlisaa_kopioiden(strlista* l, char* str);
strlista* _strlisaa_kopioiden_taakse(strlista* l, char* str);
strlista* _strlistaksi(char* s, const char* erotin);
strlista* _strnouda(strlista*, int);
strlista* _strkopioi(strlista*, int n);
void      _strpoista(strlista*, int*, int);
strlista* _strpoista1(strlista*, char);
strlista* _strpoista_kaikki(strlista*);
void      _strjarjesta(strlista*);
void      _strvapautus(void*);
