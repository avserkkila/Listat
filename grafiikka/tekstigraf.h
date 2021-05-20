#ifndef __TEKSTIGRAFIIKKA__
#define __TEKSTIGRAFIIKKA__
#include <strlista.h>
#include <SDL.h>
#include <SDL_ttf.h>

typedef struct {
  char* teksti;
  strlista* tkstil;
  char ttflaji; //mitä ttf-kirjaston funktiota käytetään (katso laita_teksti_ttf())
  TTF_Font* font;
  int fonttikoko;
  char* fonttied;
  SDL_Rect* sij; //tälle varattu maksimitila
  SDL_Rect* toteutuma; //mikä tila oikeasti käytetään
  SDL_Color vari;
  short alku; //koskee vain listoja, ensimmäisen näytetyn indeksi, 'r'
  short rullaus; //koskee vain listoja, 'w'
  char numerointi; //koskee vain listoja;
} tekstiolio_s;

#endif

void laita_teksti_ttf(tekstiolio_s*, SDL_Renderer*);
int laita_tekstilista(strlista*, int, tekstiolio_s*, SDL_Renderer*);
int laita_pari_oikealle(tekstiolio_s* ov, int vali,		\
			strlista* a, strlista* b, int alku,	\
			tekstiolio_s* o, SDL_Renderer* rend);
void laita_vierekkain(strlista* a, strlista* b, int alku, tekstiolio_s* o, SDL_Renderer* r);
void laita_oikealle(tekstiolio_s* ov, short vali, strlista* l, int alku, tekstiolio_s* o, SDL_Renderer* r);
void laita_teksti_ttf_vasemmalle(tekstiolio_s* ov, short vali, tekstiolio_s* o, SDL_Renderer* r);