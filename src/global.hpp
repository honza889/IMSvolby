#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include "simlib.h"

extern bool nakratko;
extern int celkovyPocetOkrsku;
extern int pocetPrebiracichMist;
extern int planovanyKonecZacatkuScitani;
extern int dobaScitani;

extern Facility vpn;
extern Facility serverCentra;
extern Histogram dobaMeziPrichodyOkrsky;
extern Histogram dobaPraceKomiseOverovani;
extern Histogram volebniUcastOkrsky;
extern Histogram dobaPraceKomiseScitani;
extern Histogram dobaCekaniNaPorizeni;
extern Histogram dobaCekaniNaCentrum;
extern Histogram dobaPozadavku;

class Kraj;
class PrebiraciMisto;
class Okrsek;
class UzavreniMistnosti;

double NormalMinMax(double min, double max);

#endif
