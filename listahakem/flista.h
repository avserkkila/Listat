#ifndef _flista_h_
#define _flista_h_
typedef struct flista_s {
  struct flista_s *edel;
  float f;
  struct flista_s *seur;
} flista;
#endif

flista* _fuusi_lista();
flista* _fjatka(flista*);
flista* _fjatka_taakse(flista*);
flista* _flisaa(flista*, float);
flista* _flisaa_taakse(flista*, float);
void    _ftulosta(flista*, const char* muoto, int);
flista* _fkopioi(flista*, int);
flista* _fkopioi_palauta_alku(flista*, int);
