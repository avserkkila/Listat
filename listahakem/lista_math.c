#include <stdlib.h>
#include "lista_math.h"
#include <math.h>
#include "listat.h"
#include <string.h>

/*kaikissa voi antaa arvon n,
  jonka täyttyessä toimintoa ei enää suoriteta*/

int intsumma(ilista* l, int n) {
  int i=0;
  while(l && n--) {
    i += l->i;
    l = l->seur;
  }
  return i;
}

double floatsumma(flista* l, int n) {
  double s=0;
  while(l && n--) {
    s += l->f;
    l = l->seur;
  }
  return s;
}

double intmean(ilista* l, int n) {
  int s = 0;
  int i = 0;
  while(l && n--) {
    s += l->i;
    i++;
    l = l->seur;
  }
  return s*1.0 / i;
}

double floatmean(flista* l, int n) {
  double s = 0;
  int i = 0;
  while(l && n--) {
    if(l->f != INFINITY && l->f != -INFINITY && l->f != NAN){
      s += l->f;
      i++;
    }
    l = l->seur;
  }
  return s / i;
}

double floatstd(flista* l, int n) {
  double ka = floatmean(l, n);
  double s = 0;
  int i = 0;
  while(l && n--) {
    if(l->f != INFINITY && l->f != -INFINITY && l->f != NAN) {
      s += pow(l->f - ka, 2);
      i++;
    }
    l = l->seur;
  }
  return sqrt(s/(i-1));
}

floatint floatmed(flista* fl, int n) {
  floatint r;
  flista* l = _yalkuun(_fkopioi(fl, n));
  int pit = _ylaske(l);
  if(!pit) {
    r.a = NAN;
    r.b = 0;
    return r;
  }
  int* ind = malloc(pit * sizeof(int));
  l = _yalkuun(floatjarjesta(l, floatmin, ind, -1));
  pit--;
  r.b = ind[pit / 2];
  l = _ynouda(l, pit/2);
  if( !(pit % 2) )
    r.a = l->f;
  else
    r.a = (l->f + l->seur->f) / 2;
  _yrma(_yalkuun(l));
  free(ind);
  return r;
}

int2 intmax(ilista* l, int n) {
  if(!l || !n)
    return (int2){-1, 0};
  int max = l->i;
  int maxind = 0;
  int ind = 0;
  while( ((l = l->seur)) && --n) {
    ind++;
    if (l->i > max) {
      max = l->i;
      maxind = ind;
    }
  }
  return (int2){max, maxind};
}

floatint floatmax(flista* l, int n) {
  if(!l || !n)
    return (floatint){NAN, 0};
  float max = l->f;
  int maxind = 0;
  int ind = 0;
  while( ((l = l->seur)) && --n) {
    ind++;
    if(isnan(l->f))
      continue;
    if (l->f > max || isnan(max)) {
      max = l->f;
      maxind = ind;
    }
  }
  return (floatint){max, maxind};
}

int2 intmin(ilista* l, int n) {
  if(!l || !n)
    return (int2){-1, 0};
  int min = l->i;
  int minind = 0;
  int ind = 0;
  while( ((l = l->seur)) && --n) {
    ind++;
    if (l->i < min) {
      min = l->i;
      minind = ind;
    }
  }
  return (int2){min, minind};
}

floatint floatmin(flista* l, int n) {
  if(!l || !n)
    return (floatint){NAN, 0};
  float min = l->f;
  int minind = 0;
  int ind = 0;
  while( ((l = l->seur)) && --n) {
    ind++;
    if(isnan(l->f))
      continue;
    if (l->f < min || isnan(min)) {
      min = l->f;
      minind = ind;
    }
  }
  return (floatint){min, minind};
}

/*j:hin tulostetaan järjestys, joka luotiin.
  j:n paikalle voi antaa NULL:in, jolloin järjestystä ei tulosteta*/
void intjarjesta(ilista* l, int2 fun(ilista*, int), int* j, int n) {
  int2 m;
  int apu = _ylaske(l);
  if(j)
    for(int i=0; i<apu; i++)
      j[i] = i;
  while(l && n--) {
    m = fun(l, n);
    ((ilista*)_ynouda(l, m.b))->i = l->i;
    l->i = m.a;
    l = l->seur;
    if(j) {
      apu = j[m.b];
      j[m.b] = j[0];
      j[0] = apu;
      j++;
    }
  }
  return;
}

flista* floatjarjesta(flista* l, floatint fun(flista*, int), int* j, int n) {
  flista* muisti = l;
  floatint m;
  int pit = _ylaske(l);
  if(j)
    for(int i=0; i<pit; i++)
      j[i] = i;
  while(l && n--) {
    m = fun(l, n);
    ((flista*)_ynouda(l, m.b))->f = l->f;
    l->f = m.a;
    l = l->seur;
    if(j) {
      float apu = j[m.b];
      j[m.b] = j[0];
      j[0] = apu;
      j++;
    }
  }
  return muisti;
}

/*mukaan vasemmalta, mukaan edestä, pois pieniä, pois suuria
  (liukuva karsittu keskiarvo)*/
float floatavg(flista* l, int mv, int mo, int pp, int ps) {
  float avg = 0;
  l = _ynouda(l, -1*mv);
  if(!l)
    return NAN;
  l = (flista*)_yalkuun(_fkopioi(l, mv+1+mo));
  l = (flista*)_yalkuun(floatjarjesta(l, floatmin, NULL, -1));
  flista* alku = l;
  flista* loppu = (flista*)_ynouda(_yloppuun(l), -1*ps+1);
  l = (flista*)_ynouda(l, pp);
  while(l && (l != loppu)) {
    avg += l->f;
    l = l->seur;
  }
  _yrma(alku);
  int n = mv+1+mo-pp-ps;
  return avg/n;
}

flista* floatmovavg(flista* l, int mv, int mo, int pp, int ps, int n) {
  flista* tulos = NULL;
  while(l && n--) {
      tulos = _flisaa(tulos, floatavg(l, mv, mo, pp, pp));
      l = l->seur;
  }
  return tulos;
}

ilista* intlistalle(ilista* l, int mv, int mo, int pp, int ps) {
  ilista* r = NULL;
  l = _ynouda(l, -1*mv);
  if(!l)
    return NULL;
  r = _ikopioi(l, mv+1+mo);
  int2 m;
  for(int i=0; i<pp; i++) {
    m = intmin(r, -1);
    if(m.b)
      _yrm(r, &(m.b), 1);
    else
      r = _yrm(r, &(m.b), 1);
  }
  for(int i=0; i<ps; i++) {
    m = intmax(r, -1);
    if(m.b)
      _yrm(r, &(m.b), 1);
    else
      r = _yrm(r, &(m.b), 1);
  }
  return r;
}

flista* floatlistalle(flista* l, int mv, int mo, int pp, int ps) {
  flista* r = NULL;
  l = _ynouda(l, -1*mv);
  if(!l)
    return NULL;
  r = _fkopioi(l, mv+1+mo);
  floatint m;
  for(int i=0; i<pp; i++) {
    m = floatmin(r, -1);
    if(m.b)
      _yrm(r, &(m.b), 1);
    else
      r = _yrm(r, &(m.b), 1);
  }
  for(int i=0; i<ps; i++) {
    m = floatmax(r, -1);
    if(m.b)
      _yrm(r, &(m.b), 1);
    else
      r = _yrm(r, &(m.b), 1);
  }
  return r;
}