#include <stdlib.h>
#include <stdio.h>
#include "flista.h"
#include "/home/antterkk/c/ylista.h"

flista* _fuusi_lista() {
  flista* l = malloc(sizeof(flista));
  if(!l)
    return NULL;
  *l = (flista) {NULL, 0, NULL};
  return l;
}

flista* _fjatka(flista* l) {
  return _ylsvlms(l, _fuusi_lista());
}

flista* _fjatka_taakse(flista* l) {
  return _ylsvlms_taakse(l, _fuusi_lista());
}

flista* _flisaa(flista *l, float f) {
  flista* uusi = _fuusi_lista();
  uusi->f = f;
  return _ylsvlms(l, uusi);
}

flista* _flisaa_taakse(flista *l, float f) {
  flista* uusi = _fuusi_lista();
  uusi->f = f;
  return _ylsvlms_taakse(l, uusi);
}

void _ftulosta(flista* l, const char* muoto, int n) {
  while(l && n--) {
    printf(muoto, l->f);
    l = l->seur;
  }
}

flista* _fkopioi(flista *l, int n) {
  flista *uusi = NULL;
  while(l && n--) {
    uusi = _flisaa(uusi, l->f);
    l = l->seur;
  }
  return uusi;
}
