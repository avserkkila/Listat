#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ylista.h"
#include "ilista.h"
#include "flista.h"

#define L ((ylista*)l)

void tuhja(void*);

void* _yuusi_lista() {
  ylista* l = malloc(sizeof(ylista));
  if(!l)
    return NULL;
  l->seur = NULL;
  l->p = NULL;
  l->edel = NULL;
  return l;
}

/*lisää annetun jäsenen perään ja palauttaa lisätyn
  jos listaa ei ole, luodaan uusi*/
void* _ylisaa(void *l, void *ptr) {
  ylista* uusi = _yuusi_lista();
  if (!uusi) return NULL;
  uusi->p = ptr;
  
  if(!l) //ei ole valmista listaa joten palautetaan vain tämä
    return uusi;
  return _ylsvlms(l, uusi);
}

/*lisää listalle valmiin jäsenen ja palauttaa viitteen siihen*/
void* _ylsvlms(void *lv, void *jv) {
  ylista *l = (ylista*)lv;
  ylista *j = (ylista*)jv;
  if(!l)
    return j;
  /*jos on olemassa seuraava jäsen, se linkitetään tähän*/
  if(l->seur) {
    l->seur->edel = j;
    j->seur       = l->seur;
  }
  l->seur = j;
  j->edel = l;
  return j;
}

void* _ylisaa_taakse(void *l, void *ptr) {
  ylista* uusi = _yuusi_lista();
  if (!uusi) return NULL;
  uusi->p = ptr;
  
  if(!l) //ei ole valmista listaa joten palautetaan vain tämä
    return uusi;
  return _ylsvlms_taakse(l, uusi);
}

void* _ylsvlms_taakse(void* lv, void* jv) {
  ylista *l = (ylista*)lv;
  ylista *j = (ylista*)jv;
  if(!l)
    return j;
  /*jos on olemassa edellinen jäsen, se linkitetään lisättävään*/
  if(l->edel) {
    l->edel->seur = j;
    j->edel       = l->edel;
  }
  l->edel = j;
  j->seur = l;
  return j;
}

/*luo listalle uuden jäsenen ja palauttaa viitteen siihen*/
void* _yjatka(void* lv) {
  return _ylsvlms(lv, _yuusi_lista());
}

void* _yjatka_taakse(void* lv) {
  return _ylsvlms_taakse(lv, _yuusi_lista());
}

void* _ynouda(void* lv, int nro) {
  if(!lv)
    return NULL;
  ylista *l = (ylista*)lv;
  if (nro < 0) {
    while(nro++ && (l = l->edel));
    return l;
  }
  while(nro-- && (l = l->seur));
  return l;
}

void* _ynoudaf(void* lv, int nro, int alku) {
  ylista *l = (ylista*)lv;
  if(alku == 0)
    l = _yalkuun(l);
  else if(alku == 1)
    l = l;
  else if(alku == 2)
    l = _yloppuun(l);
  else
    return NULL;
  return _ynouda(l, nro);
}

void* _yalkuun(void* lv) {
  if(!lv)
    return NULL;
  ylista *l = (ylista*)lv;
  if(l)
    while (l->edel)
      l = l->edel;
  return l;
}

void* _yloppuun(void* lv) {
  if(!lv)
    return NULL;
  ylista *l = (ylista*)lv;
  if(l)
    while (l->seur)
      l = l->seur;
  return l;
}

int _ylaske(void* lv) {
  ylista *l = (ylista*)lv;
  if (!l)
    return 0;
  int r = 1;
  while((l = l->seur))
    r++;
  return r;
}

int _ylaske_taakse(void* lv) {
  ylista *l = (ylista*)lv;
  if (!l)
    return 0;
  int r = 1;
  while((l = l->edel))
    r++;
  return r;
}

int _yind(void* lv) {
  if(!lv)
    return -1;
  ylista* juokseva = _yalkuun(lv);
  int r = 0;
  while(juokseva != lv) {
    r++;
    juokseva = juokseva->seur;
  }
  return r;
}

/*palauttaa jäljelle jääneen listan ensimmäisen jäsenen*/
void* _ypoista(void* vl, int* nrot, int kpl, void (*vapautus)(void*)) {
  ylista* l = (ylista*)vl;
  ylista *poistot[kpl];
  for(int i=0; i<kpl; i++)
    poistot[i] = _ynouda(l, nrot[i]);
  /*Jos poistetaan alku, johon l osoittaa, siirretään l uuteen alkuun*/
  for(int i=0; i<kpl; i++) {
    if (poistot[i] == l)
      l = _ypoista1(poistot[i], vapautus, 1);
    else
      _ypoista1(poistot[i], vapautus, 1);
  }
  return l;
}

/*palauttaa ensimmäisen, jota ei poistettu*/
void* _ypoista_maski(void* vl, char* maski, void (*vapautus)(void*)) {
  ylista* l = (ylista*)vl;
  ylista* r = l;
  int i=0;
  while(l)
    if(maski[i]) {
      if(r == l)
	r = l->seur;
      l = _ypoista1(l, vapautus, 1);
    }
    else
      l = l->seur;
  return r;
}

void* _yrm(void* lv, int* nrot, int kpl) {
  return _ypoista((ylista*)lv, nrot, kpl, tuhja);
}

void* _yrm1(void* pv, char s) {
  return _ypoista1((ylista*)pv, tuhja, s);
}

void _yrma(void* pv) {
  _ypoista_kaikki((ylista*)pv, tuhja);
}

/*Poistaa annetun jäsenen ja palauttaa seuraavan (s>=0) tai edellisen (s<0)*/
void* _ypoista1(void* vptr, void (*vapautus)(void*), int s) {
  if(!vptr)
    return NULL;
  ylista* ptr = (ylista*)vptr;
  ylista* r;
  if(s < 0)
    r = ptr->edel;
  else
    r = ptr->seur;
  if (ptr->edel)
    ptr->edel->seur = ptr->seur;
  if (ptr->seur)
    ptr->seur->edel = ptr->edel;
  vapautus(ptr);
  free(ptr);
  return r;
}

void* _ypoista_kaikki(void* vptr, void (*vapautus)(void*)) {
  if(!vptr)
    return NULL;
  if( ((ylista*)vptr)->edel )
    ((ylista*)vptr)->edel->seur = NULL;
  do
    vptr = _ypoista1(vptr, vapautus, 1);
  while(vptr);
  return NULL;
}

void* _yjarjestapit(void* l, int* j, int pit) {
  void **a = malloc(pit*sizeof(void*));
  void* l0 = l;
  for(int i=0; i<pit; i++)
    a[i] = ((ylista*)_ynouda(l, j[i]))->p;
  for(int i=0; i<pit; i++) {
    L->p = a[i];
    l = L->seur;
  }
  free(a);
  return l0;
}

/*Näitä tulostusjuttuja ei kannata käyttää.
  Koodattakoon kullekin listalle oma tulostusfunktionsa*/

void _ytulosta(void *vl, char* muoto) {
  ylista* l = (ylista*)vl;
  if (strstr(muoto,"%s"))
    {
      while(l) {
	printf(muoto, (char*)l->p);
	l=l->seur;
      }
    }
  else
    {
      while(l) {
	printf(muoto, *(int*)(l->p));
	l=l->seur;
      }
    }
}

void _yftulosta(FILE *f, void *vl, char* muoto) {
  ylista* l = (ylista*)vl;
  if (strstr(muoto,"%s"))
    {
      while(l) {
	fprintf(f, muoto, (char*)l->p);
	l=l->seur;
      }
    }
  else
    {
      while(l) {
	fprintf(f, muoto, *(int*)(l->p));
	l=l->seur;
      }
    }
}

void _ytulostaf(void *vl, char* muoto, int x) {
  ylista* l = (ylista*)vl;
  if(strstr(muoto,"%s"))
    {
      while(l) {
	for(int i=0; (i<x && l); i++) {
	  printf(muoto, (char*)l->p);
	  putchar(' ');
	  l=l->seur;
	}
	putchar('\n');
      }
    }
  else
    {
      while(l) {
	for(int i=0; (i<x && l); i++) {
	  printf(muoto, *(int*)(l->p));
	  putchar(' ');
	  l=l->seur;
	}
	putchar('\n');
      }
    }
}

void tuhja(void* v) {
  ;
}
