#include <stdlib.h>
#include <omp.h>
#include "lista_math.h"
#include <math.h>
#include "listat.h"
#include <string.h>

/*useimmissa voi antaa arvon n,
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
  unsigned* ind = malloc(pit * sizeof(unsigned));
  l = float_lomitusjarj_jarj_pit(l, ind, pit);
  pit--;
  r.b = ind[pit / 2];
  flista* l1 = _ynouda(l, pit/2);
  if( !(pit % 2) )
    r.a = l1->f;
  else
    r.a = (l1->f + l1->seur->f) / 2;
  _yrma(l);
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

ilista* int_lomitusjarj(ilista* l) {
  int_lomitusjarj_pit(l, _ylaske(l));
  return l;
}

/*lomitusjärjestäminen (merge sort), jossa pituus annetaan argumenttina*/
ilista* int_lomitusjarj_pit(ilista* l0, unsigned n) {
  if(n < 2)
    return l0;
  int_lomitusjarj_pit_yli1(l0, n);
  return l0;
}

/*optimoinnin vuoksi palauttaa viimeisen jäsenen
  ja pituuden ollessa enintään 3 käyttää stooge-lajittelua*/

ilista* int_lomitusjarj_pit_yli1(ilista* l0, unsigned n) {
  if(n <= 3) {
    ilista* l1 = l0->seur;
    int iarr[] = {l0->i, l1->i};
    int pienempi = iarr[1] < iarr[0];
    l0->i = iarr[pienempi];
    l1->i = iarr[!pienempi];
    if(n==2)
      return l1;
    ilista* l2 = l1->seur;
    iarr[0] = l1->i; iarr[1] = l2->i;
    pienempi = iarr[1] < iarr[0];
    l1->i = iarr[pienempi];
    l2->i = iarr[!pienempi];
    
    iarr[0] = l0->i; iarr[1] = l1->i;
    pienempi = iarr[1] < iarr[0];
    l0->i = iarr[pienempi];
    l1->i = iarr[!pienempi];
    return l2;
  }
  
  /*puolikkaitten järjestäminen*/
  unsigned raja = n>>1; // n/2
  ilista* l[2];
  l[0] = l0;
  l[1] = int_lomitusjarj_pit_yli1(l0, raja)->seur;
  ilista* viimeinen = int_lomitusjarj_pit_yli1(l[1], n-raja);
  
  int* muisti = malloc(n*sizeof(int));
  int id=0;
  int pienempi;
  
  /*katkaistaan viimeisistä linkit seuraaviin*/
  ilista* viimeinen_seur = viimeinen->seur;
  viimeinen->seur = NULL;
  l[1]->edel->seur = NULL;
  ilista* l1_alussa = l[1];
  /*lomitus*/
  while(1) {
    pienempi = l[1]->i < l[0]->i;
    muisti[id++] = l[pienempi]->i;
    if(!l[pienempi]->seur)
      break;
    l[pienempi] = l[pienempi]->seur;
  }
  /*kopioidaan loput*/
  ilista* juoksu = l[!pienempi];
  while(juoksu) {
    muisti[id++] = juoksu->i;
    juoksu = juoksu->seur;
  }
  /*palautetaan linkit seuraaviin*/
  viimeinen->seur = viimeinen_seur;
  l1_alussa->edel->seur = l1_alussa;
  /*kopioidaan taulukko takaisin listaan*/
  for(int tmp=0; tmp<id; tmp++) {
    l0->i = muisti[tmp];
    l0 = l0->seur;
  }
  free(muisti);
  return viimeinen;
}

ilista* int_lomitusjarj_jarj(ilista* l0, unsigned* taul) {
  return int_lomitusjarj_jarj_pit(l0, taul, _ylaske(l0));
}

/*kuin int_lomitusjarj_pit, mutta taulukkoon taul laittaa syntyneen järjestyksen*/
ilista* int_lomitusjarj_jarj_pit(ilista* l0, unsigned* taul, unsigned n) {
  for(unsigned i=0; i<n; i++)
    taul[i] = i;
  if(n < 2)
    return l0;
  int_lomitusjarj_jarj_pit_yli1(l0, taul, n);
  return l0;
}

ilista* int_lomitusjarj_jarj_pit_yli1(ilista* l0, unsigned* taul, unsigned n) {
  if(n <= 3) {
    ilista* l1 = l0->seur;
    int iarr[] = {l0->i, l1->i};
    int pienempi = iarr[1] < iarr[0];
    l0->i = iarr[pienempi];
    l1->i = iarr[!pienempi];
    int taularr[] = {taul[0], taul[1]};
    taul[0] = taularr[pienempi];
    taul[1] = taularr[!pienempi];
    if(n==2)
      return l1;
    ilista* l2 = l1->seur;
    iarr[0] = l1->i; iarr[1] = l2->i;
    pienempi = iarr[1] < iarr[0];
    l1->i = iarr[pienempi];
    l2->i = iarr[!pienempi];
    taularr[0] = taul[1]; taularr[1] = taul[2];
    taul[1] = taularr[pienempi];
    taul[2] = taularr[!pienempi];

    iarr[0] = l0->i; iarr[1] = l1->i;
    pienempi = iarr[1] < iarr[0];
    l0->i = iarr[pienempi];
    l1->i = iarr[!pienempi];
    taularr[0] = taul[0]; taularr[1] = taul[1];
    taul[0] = taularr[pienempi];
    taul[1] = taularr[!pienempi];
    return l2;
  }
  
  /*puolikkaitten järjestäminen*/
  unsigned raja = n>>1; // n/2
  ilista* l[2];
  l[0] = l0;
  l[1] = int_lomitusjarj_jarj_pit_yli1(l0, taul, raja)->seur;
  ilista* viimeinen = int_lomitusjarj_jarj_pit_yli1(l[1], taul+raja, n-raja);

  int* muisti = malloc(n*sizeof(int));
  int id=0; int pienempi;
  unsigned* muistitaul = malloc(n*sizeof(unsigned));
  for(int i=0; i<n; i++)
    muistitaul[i] = taul[i];
  unsigned* t[] = {muistitaul, muistitaul+raja};

  /*katkaistaan viimeisistä linkit seuraaviin*/
  ilista* viimeinen_seur = viimeinen->seur;
  viimeinen->seur = NULL;
  l[1]->edel->seur = NULL;
  ilista* l1_alussa = l[1];
  /*lomitus*/
  do {
    pienempi = l[1]->i < l[0]->i;
    muisti[id] = l[pienempi]->i;
    taul[id++] = *t[pienempi]++;
  } while((l[pienempi] = l[pienempi]->seur));
  /*kopioidaan loput*/
  ilista* juoksu = l[!pienempi];
  unsigned* ttmp = t[!pienempi];
  while(juoksu) {
    muisti[id] = juoksu->i;
    taul[id++] = *ttmp++;
    juoksu = juoksu->seur;
  }
  /*palautetaan linkit seuraaviin*/
  viimeinen->seur = viimeinen_seur;
  l1_alussa->edel->seur = l1_alussa;
  /*kopioidaan taulukko takaisin listaan*/
  for(int tmp=0; tmp<id; tmp++) {
    l0->i = muisti[tmp];
    l0 = l0->seur;
  }
  free(muisti);
  free(muistitaul);

  return viimeinen;
}

/*ERITTÄIN hidas valintalajittelu
  j:hin tulostetaan järjestys, joka luotiin.
  j:n paikalle voi antaa NULL:in, jolloin järjestystä ei tulosteta*/
void intjarjesta(ilista* l, int2 fun(ilista*, int), int* j, int n) {
  int2 m;
  int apu;
  int pit = _ylaske(l);
  if(j)
    for(int i=0; i<pit && n-i; i++)
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

flista* float_lomitusjarj(flista* l) {
  float_lomitusjarj_pit(l, _ylaske(l));
  return l;
}

/*lomituslajittelu (merge sort), jossa pituus annetaan argumenttina*/
flista* float_lomitusjarj_pit(flista* l0, unsigned n) {
  if(n < 2)
    return l0;
  float_lomitusjarj_pit_yli1(l0, n);
  return l0;
}

/*optimoinnin vuoksi palauttaa viimeisen jäsenen
  ja pituuden ollessa enintään 3 käyttää stooge-lajittelua*/
flista* float_lomitusjarj_pit_yli1(flista* l0, unsigned n) {
  if(n <= 3) {
    flista* l1 = l0->seur;
    float arr[] = {l0->f, l1->f};
    int pienempi = arr[1] < arr[0];
    l0->f = arr[pienempi];
    l1->f = arr[!pienempi];
    if(n==2)
      return l1;
    flista* l2 = l1->seur;
    arr[0] = l1->f; arr[1] = l2->f;
    pienempi = arr[1] < arr[0];
    l1->f = arr[pienempi];
    l2->f = arr[!pienempi];
    
    arr[0] = l0->f; arr[1] = l1->f;
    pienempi = arr[1] < arr[0];
    l0->f = arr[pienempi];
    l1->f = arr[!pienempi];
    return l2;
  }
  
  /*puolikkaitten järjestäminen*/
  unsigned raja = n>>1; // n/2
  flista* l[2];
  l[0] = l0;
  l[1] = float_lomitusjarj_pit_yli1(l0, raja)->seur;
  flista* viimeinen = float_lomitusjarj_pit_yli1(l[1], n-raja);
  
  float* muisti = malloc(n*sizeof(float));
  int id=0;
  int pienempi;
  
  /*katkaistaan viimeisistä linkit seuraaviin*/
  flista* viimeinen_seur = viimeinen->seur;
  viimeinen->seur = NULL;
  l[1]->edel->seur = NULL;
  flista* l1_alussa = l[1];
  /*lomitus*/
  while(1) {
    pienempi = l[1]->f < l[0]->f;
    muisti[id++] = l[pienempi]->f;
    if(!l[pienempi]->seur)
      break;
    l[pienempi] = l[pienempi]->seur;
  }
  /*kopioidaan loput*/
  flista* juoksu = l[!pienempi];
  while(juoksu) {
    muisti[id++] = juoksu->f;
    juoksu = juoksu->seur;
  }
  /*palautetaan linkit seuraaviin*/
  viimeinen->seur = viimeinen_seur;
  l1_alussa->edel->seur = l1_alussa;
  /*kopioidaan taulukko takaisin listaan*/
  for(int tmp=0; tmp<id; tmp++) {
    l0->f = muisti[tmp];
    l0 = l0->seur;
  }
  free(muisti);
  return viimeinen;
}

flista* float_lomitusjarj_jarj(flista* l0, unsigned *taul) {
  return float_lomitusjarj_jarj_pit(l0, taul, _ylaske(l0));
}

/*kuin float_lomitusjarj_pit, mutta taulukkoon taul laittaa syntyneen järjestyksen*/
flista* float_lomitusjarj_jarj_pit(flista* l0, unsigned* taul, unsigned n) {
  for(unsigned i=0; i<n; i++)
    taul[i] = i;
  if(n < 2)
    return l0;
  float_lomitusjarj_jarj_pit_yli1(l0, taul, n);
  return l0;
}

flista* float_lomitusjarj_jarj_pit_yli1(flista* l0, unsigned* taul, unsigned n) {
  if(n <= 3) {
    flista* l1 = l0->seur;
    float iarr[] = {l0->f, l1->f};
    int pienempi = iarr[1] < iarr[0];
    l0->f = iarr[pienempi];
    l1->f = iarr[!pienempi];
    int taularr[] = {taul[0], taul[1]};
    taul[0] = taularr[pienempi];
    taul[1] = taularr[!pienempi];
    if(n==2)
      return l1;
    flista* l2 = l1->seur;
    iarr[0] = l1->f; iarr[1] = l2->f;
    pienempi = iarr[1] < iarr[0];
    l1->f = iarr[pienempi];
    l2->f = iarr[!pienempi];
    taularr[0] = taul[1]; taularr[1] = taul[2];
    taul[1] = taularr[pienempi];
    taul[2] = taularr[!pienempi];

    iarr[0] = l0->f; iarr[1] = l1->f;
    pienempi = iarr[1] < iarr[0];
    l0->f = iarr[pienempi];
    l1->f = iarr[!pienempi];
    taularr[0] = taul[0]; taularr[1] = taul[1];
    taul[0] = taularr[pienempi];
    taul[1] = taularr[!pienempi];
    return l2;
  }
  
  /*puolikkaitten järjestäminen*/
  unsigned raja = n>>1; // n/2
  flista* l[2];
  l[0] = l0;
  l[1] = float_lomitusjarj_jarj_pit_yli1(l0, taul, raja)->seur;
  flista* viimeinen = float_lomitusjarj_jarj_pit_yli1(l[1], taul+raja, n-raja);

  float* muisti = malloc(n*sizeof(float));
  int id=0; int pienempi;
  unsigned* muistitaul = malloc(n*sizeof(unsigned));
  for(int i=0; i<n; i++)
    muistitaul[i] = taul[i];
  unsigned* t[] = {muistitaul, muistitaul+raja};

  /*katkaistaan viimeisistä linkit seuraaviin*/
  flista* viimeinen_seur = viimeinen->seur;
  viimeinen->seur = NULL;
  l[1]->edel->seur = NULL;
  flista* l1_alussa = l[1];
  /*lomitus*/
  do {
    pienempi = l[1]->f < l[0]->f;
    muisti[id] = l[pienempi]->f;
    taul[id++] = *t[pienempi]++;
  } while((l[pienempi] = l[pienempi]->seur));
  /*kopioidaan loput*/
  flista* juoksu = l[!pienempi];
  unsigned* ttmp = t[!pienempi];
  while(juoksu) {
    muisti[id] = juoksu->f;
    taul[id++] = *ttmp++;
    juoksu = juoksu->seur;
  }
  /*palautetaan linkit seuraaviin*/
  viimeinen->seur = viimeinen_seur;
  l1_alussa->edel->seur = l1_alussa;
  /*kopioidaan taulukko takaisin listaan*/
  for(int tmp=0; tmp<id; tmp++) {
    l0->f = muisti[tmp];
    l0 = l0->seur;
  }
  free(muisti);
  free(muistitaul);

  return viimeinen;
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
