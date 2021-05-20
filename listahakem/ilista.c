#include <stdlib.h>
#include <stdio.h>
#include "ilista.h"
#include "ylista.h"

ilista* _iuusi_lista() {
  ilista* l = malloc(sizeof(ilista));
  if(!l)
    return NULL;
  *l = (ilista) {NULL, 0, NULL};
  return l;
}

ilista* _ilisaa(ilista *l, int i) {
  ilista* uusi = _iuusi_lista();
  uusi->i = i;
  return _ylsvlms(l, uusi);
}

ilista* _ilisaa_taakse(ilista *l, int i) {
  ilista* uusi = _iuusi_lista();
  uusi->i = i;
  return _ylsvlms_taakse(l, uusi);
}

void _itulosta(ilista* l, int n) {
  while(l && n--) {
    printf("%i\n", l->i);
    l = l->seur;
  }
}

ilista* _ikopioi(ilista *l, int n) {
  ilista *uusi;
  while(l && n--) {
    uusi = _ilisaa(uusi, l->i);
    l = l->seur;
  }
  return uusi;
}
