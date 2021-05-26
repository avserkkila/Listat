#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "strlista.h"
#include "ylista.h"

void _strtulosta(void* l) {
  _ytulosta(l, "%s\n");
}

void _strtulosta_valein(void* lv) {
  strlista* l = lv;
  printf("%s",l->str);
  _ytulosta(l->seur, " %s");
  puts("");
}

void _strftulosta(FILE *f, void* l) {
  _yftulosta(f, l, "%s\n");
}

void _strftulosta_valein(FILE *f, void* lv) {
  strlista *l = lv;
  fprintf(f, "%s",l->str);
  _yftulosta(f, l->seur, " %s");
  fputc('\n', f);
}

char _strstulostaf(char* s, char* muoto, void* lv) {
  strlista *l = lv;
  while(l) {
    if(!sprintf(s, muoto, l->str))
      return 1;
    l=l->seur;
    s = s+strlen(s);
  }
  return 0;
}

strlista* _strlistaksi(const char* s, const char* restrict erotin) {
  char* str = strdup(s);
  char* str0 = str;
  strlista* l = NULL;
  char* ptr;
  while( (ptr = strstr(str, erotin)) ) {
    *ptr = '\0';
    l = _strlisaa_kopioiden(l, str);
    str += strlen(str) + strlen(erotin);
  }
  /*viimeinen ei pääty erottimeen*/
  l = _strlisaa_kopioiden(l,str);
  free(str0);
  return l;
}

strlista* _strpilko_sanoiksi(const char* restrict s) {
  strlista *l = NULL;
  char *sana = malloc(strlen(s)+1);
  while(sscanf(s, "%s", sana) > 0) {
    l = _strlisaa_kopioiden(l, sana);
    while(s[0] != sana[0])
      s++;
    s += strlen(sana);
  }
  free(sana);
  return _yalkuun(l);
}

strlista* _strlisaa_kopioiden(void* lv, const char* str) {
  strlista* l = lv;
  l = _yjatka(l);
  l->str = malloc(strlen(str)+1);
  strcpy(l->str, str);
  return l;
}

strlista* _strlisaa_kopioiden_taakse(void* lv, const char* str) {
  strlista* l = lv;
  l = _yjatka_taakse(l);
  l->str = malloc(strlen(str)+1);
  strcpy(l->str, str);
  return l;
}

strlista* _strnouda(void* l, int i) {
  return (strlista*)_ynouda(l, i);
}

strlista* _strkopioi(void* vanhav, int n) {
  strlista *vanha = vanhav;
  strlista *uusi = NULL;
  while(vanha && n--) {
    uusi = _strlisaa_kopioiden(uusi, vanha->str);
    vanha = vanha->seur;
  }
  return _yalkuun(uusi);
}

strlista* _strpoista(void* l, int* nrot, int kpl) {
  return _ypoista(l, nrot, kpl, _strvapautus);
}

void* _strpoista_kaikki(void* l) {
  _ypoista_kaikki(l, _strvapautus);
  return NULL;
}

strlista* _strpoista1(void* l, char s) {
  return (strlista*)_ypoista1(l, _strvapautus, s);
}

void _strvapautus(void* l) {
  free(((strlista*)l)->str);
}
  
  
int oma_strvrt(char*, char*);
int oma_strvrt1(char *a, char *b);

/*Järjestää vaihtamalla vain p-pointterien osoitteet
  Vaihtaa ensimmäisen paikalleen ja sitten rekursio seuraavasta alkaen*/
void _strjarjesta(void* lv) {
  if(!lv) return;
  strlista *l = lv;
  strlista *alkumuisti  = l;
  strlista *ensimmainen = l;
  
  while((l = l->seur))
    if(oma_strvrt(l->str, ensimmainen->str) < 0)
      ensimmainen = l;

  char *apu        = ensimmainen->str;
  ensimmainen->str = alkumuisti->str;
  alkumuisti ->str = apu;
  
  _strjarjesta(alkumuisti->seur);
}

int oma_strvrt(char *a, char *b) {
  /*Laitetaan isot ja pienet kirjaimet limittäin
    AaBbCc ... ÅåÄäÖö. En ole vielä lisännyt ääkkösiä tähän*/
  int vrt = 0;
  while (*a && *b && !(vrt = oma_strvrt1(a++, b++)));
  return vrt;
}

int oma_strvrt1(char *a, char *b) {
  double aa, bb;
  /*Ensin yritetään verrata lukuina*/
  if ( sscanf(a, "%lf", &aa) && sscanf(b, "%lf", &bb) )
    return aa - bb;
  /*Ja sitten kirjaimina*/
  aa = *a;
  bb = *b;
  if (*a >= 'A' && *a <= 'Z')
    aa = *a + ('a' - 'A') - 0.5;
  if (*b >= 'A' && *b <= 'Z')
    bb = *b + ('a' - 'A') - 0.5;
  return (aa - bb) * 2;
}
