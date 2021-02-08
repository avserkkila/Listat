#ifndef _ilista_h_
#define _ilista_h_
typedef struct ilista_s {
  struct ilista_s *edel;
  int i;
  struct ilista_s *seur;
} ilista;

ilista* _iuusi_lista();
ilista* _ilisaa(ilista*, int);
void    _itulosta(ilista*, int n);
ilista* _ikopioi(ilista*, int n);

#endif
