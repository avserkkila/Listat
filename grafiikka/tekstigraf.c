#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <strlista.h>
#include "tekstigraf.h"

float skaala = 1.0;

void laita_teksti_ttf(tekstiolio_s *o, SDL_Renderer *rend) {
  if(!o->teksti || !strcmp(o->teksti, ""))
    return;
  SDL_Surface *pinta;
  switch(o->ttflaji) {
  case 0:
  OLETUSLAJI:
    pinta = TTF_RenderUTF8_Solid(o->font, o->teksti, o->vari);
    break;
  case 1:
    pinta = TTF_RenderUTF8_Shaded(o->font, o->teksti, o->vari, (SDL_Color){0,0,0,0});
    break;
  case 2:
    pinta = TTF_RenderUTF8_Blended(o->font, o->teksti, o->vari);
    break;
  default:
    printf("Varoitus: tekstin laittamisen laji on tuntematon, käytetään oletusta\n");
    goto OLETUSLAJI;
  }
  if(!pinta) {
    fprintf(stderr, "Virhe tekstin luomisessa: %s\n", TTF_GetError());
    return;
  }
  o->ttuurit = _ylisaa(o->ttuurit, SDL_CreateTextureFromSurface(rend, pinta));
  if(!o->ttuurit->p)
    fprintf(stderr, "Virhe tekstuurin luomisessa: %s\n", SDL_GetError());

  /*kuvan koko on luodun pinnan koko, mutta enintään objektille määritelty koko
    tulostetaan vain se osa lopusta, joka mahtuu kuvaan*/

  *(o->toteutuma) = (SDL_Rect){o->sij->x*skaala,			\
			       o->sij->y*skaala,			\
			       (pinta->w < o->sij->w)? pinta->w : o->sij->w, \
			       (pinta->h < o->sij->h)? pinta->h : o->sij->h};
  o->toteutuma->w *= skaala;
  o->toteutuma->h *= skaala;

  o->osat = _ylisaa(o->osat, malloc(sizeof(SDL_Rect)));
  o->totmat = _ylisaa(o->totmat, malloc(sizeof(SDL_Rect)));
  *(SDL_Rect*)(o->osat->p) = (SDL_Rect){(pinta->w < o->sij->w)? 0 : pinta->w - o->toteutuma->w, \
					(pinta->h < o->sij->h)? 0 : pinta->h - o->toteutuma->h, \
					pinta->w,			\
					pinta->h};
  *(SDL_Rect*)(o->totmat->p) = *(o->toteutuma);
  
  SDL_FreeSurface(pinta);
  return;
}

/*antamalla aluksi (alku) 0:n lista tulostetaan alkupäästä, muuten loppupäästä
  palauttaa, montako laitettiin*/
int laita_tekstilista(strlista* l, int alku, tekstiolio_s *o, SDL_Renderer *rend) {
  if(!l) {
    o->toteutuma->w = 0;
    o->toteutuma->h = 0;
    return 0;
  }
  int rvali = TTF_FontLineSkip(o->font);
  int mahtuu = o->sij->h / rvali;
  int yht = _ylaske(l) - o->rullaus;
  /*tässä toteutumaksi tulee maksimit*/
  int maksw = 0;
  int montako = 0;
  
  /*laitetaan niin monta jäsentä kuin mahtuu*/
  if(alku) //laitetaan lopusta
    alku = (mahtuu > yht)? 0 : (yht - mahtuu);
  else //laitetaan alusta
    alku = -o->rullaus;
  o->alku = alku;
  l = _ynouda(l, alku);
  int oy = o->sij->y;
  for(int i=0; i<mahtuu && l; i++) {
    if(o->numerointi) {
      o->teksti = malloc(strlen(l->str)+10);
      sprintf(o->teksti, "%i. %s", alku+1+i, l->str);
    } else {
      o->teksti = l->str;
    }
    laita_teksti_ttf(o, rend);
    montako++;
    if(o->toteutuma->w > maksw)
      maksw = o->toteutuma->w;
    (o->sij->y) += rvali;
    l = l->seur;
    if(o->numerointi)
      free(o->teksti);
  }
  o->toteutuma->x = o->sij->x;
  o->toteutuma->y = oy;
  o->toteutuma->w = maksw;
  o->toteutuma->h = o->sij->y - oy;
  o->sij->y = oy;
  return montako;
}

/*olion oikealle laitetaan kaksi listaa yhdessä (erikseen numerointi ja ajat)*/
int laita_pari_oikealle(tekstiolio_s* ov, int vali,		\
			   strlista* l1, strlista* l2, int alku,	\
			   tekstiolio_s* o, SDL_Renderer* rend) {
  SDL_Rect sij0 = *(o->sij);
  SDL_Rect tot1;
  int uusi_x = ov->toteutuma->x + ov->toteutuma->w + vali;
  if(o->sij->x < uusi_x)
    o->sij->x = uusi_x;
  
  int montako = laita_tekstilista(l1, alku, o, rend);
  tot1 = *(o->toteutuma);
  o->sij->x = o->toteutuma->x + o->toteutuma->w;
  o->sij->w -= o->toteutuma->w;
  laita_tekstilista(l2, alku, o, rend);
  *(o->sij) = sij0;
  o->toteutuma->x = tot1.x;
  o->toteutuma->w += tot1.w;
  return montako;
}

/*tämä palauttaa toteutumaksi näitten yhteisen alueen*/
void laita_vierekkain(strlista* a, strlista* b, int alku, tekstiolio_s* o, SDL_Renderer* r) {
  laita_tekstilista(a, alku, o, r);
  SDL_Rect sij0 = *(o->sij);
  SDL_Rect tot0 = *(o->toteutuma);
  
  o->sij->x = o->toteutuma->x + o->toteutuma->w;
  o->sij->y = o->toteutuma->y;
  o->sij->w = o->sij->w - o->toteutuma->w;
  o->sij->h = o->toteutuma->h;

  laita_tekstilista(b, alku, o, r);
  
  *(o->sij) = sij0;
  o->toteutuma->x = tot0.x;
  o->toteutuma->w += tot0.w;
  if(o->toteutuma->h < tot0.h)
    o->toteutuma->h = tot0.h;
}

void laita_oikealle(tekstiolio_s* ov, short vali, strlista* l, int alku, tekstiolio_s* o, SDL_Renderer* r) {
  if(!o)
    o = ov;
  int vanha_x = o->sij->x;
  int uusi_x = ov->toteutuma->x + ov->toteutuma->w + vali;
  if(o->sij->x < uusi_x)
    o->sij->x = uusi_x;
  laita_tekstilista(l, alku, o, r);
  o->sij->x = vanha_x;
  return;
}

void laita_alle(tekstiolio_s* yll, int vali, strlista* l, tekstiolio_s* o, SDL_Renderer* r) {
  int uusi_y = yll->toteutuma->y + yll->toteutuma->h + vali;
  o->sij->y = uusi_y;
  o->sij->x = yll->toteutuma->x;
  laita_tekstilista(l, o->lopusta, o, r);
}

void laita_teksti_ttf_vasemmalle(tekstiolio_s* ov, short vali, tekstiolio_s* o, SDL_Renderer* r) {
  if(!o)
    o = ov;
  if(!o->teksti || !strcmp(o->teksti, ""))
    return;
  SDL_Surface *pinta;
  switch(o->ttflaji) {
  case 0:
  OLETUSLAJI:
    pinta = TTF_RenderUTF8_Solid(o->font, o->teksti, o->vari);
    break;
  case 1:
    pinta = TTF_RenderUTF8_Shaded(o->font, o->teksti, o->vari, (SDL_Color){0,0,0,0});
    break;
  case 2:
    pinta = TTF_RenderUTF8_Blended(o->font, o->teksti, o->vari);
    break;
  default:
    printf("Varoitus: tekstin laittamisen laji on tuntematon, käytetään oletusta\n");
    goto OLETUSLAJI;
  }
  if(!pinta) {
    fprintf(stderr, "Virhe tekstin luomisessa: %s\n", TTF_GetError());
    return;
  }
  SDL_Texture *ttuuri = SDL_CreateTextureFromSurface(r, pinta);
  if(!ttuuri)
    fprintf(stderr, "Virhe tekstuurin luomisessa: %s\n", SDL_GetError());

  /*kuvan koko on luodun pinnan koko, mutta enintään objektille määritelty koko
    tulostetaan vain se osa lopusta, joka mahtuu kuvaan*/

  /*kumpi tahansa, x tai w voi rajoittaa tätä*/
  int yrite = ov->sij->x - vali - pinta->w;
  if(pinta->w > o->sij->w)
    yrite = ov->sij->x - vali - o->sij->w;
  *(o->toteutuma) = (SDL_Rect){(yrite > o->sij->x)? yrite : o->sij->x,	\
			       o->sij->y,				\
			       (o->sij->w < pinta->w)? o->sij->w : pinta->w, \
			       (pinta->h < o->sij->h)? pinta->h : o->sij->h};

  yrite = pinta->w - o->toteutuma->w;
  SDL_Rect osa = (SDL_Rect){(yrite>0)? yrite : 0,			\
			    (pinta->h < o->sij->h)? 0 : pinta->h - o->toteutuma->h, \
			    pinta->w - ((yrite>0)? yrite : 0),		\
			    pinta->h};

  SDL_RenderFillRect(r, o->toteutuma);
  SDL_RenderCopy(r, ttuuri, &osa, o->toteutuma);
  SDL_FreeSurface(pinta);
  SDL_DestroyTexture(ttuuri);
  return;
}

void ttuurivapautus(void* l) {
  SDL_DestroyTexture((SDL_Texture*)( (*(ylista*)l).p ));
}

void poista_ttuurit(tekstiolio_s* o) {
  o->osat = _strpoista_kaikki(_yalkuun(o->osat));
  o->totmat = _strpoista_kaikki(_yalkuun(o->totmat));
  o->ttuurit = _ypoista_kaikki(_yalkuun(o->ttuurit), ttuurivapautus);
}
