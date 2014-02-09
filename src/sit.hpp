#ifndef SIT_HPP
#define SIT_HPP

#include "simlib.h"
#include "okrsek.hpp"
#include <iostream>
#include <vector>

class Kraj {
public:
	
	const char *nazev;
	std::vector<PrebiraciMisto*> prebiraciMista;
	
	Kraj(const char* nazev):
		nazev(nazev)
	{}
	
};

class PrebiraciMisto {
public:
	
	Kraj* kraj;
	const char *nazev;
	std::vector<Okrsek*> okrsky;
	Store porizovaciStanice;
	Facility zpracovatelskaStanice;
	int pocetPorizenychOkrsku;
	
	PrebiraciMisto(Kraj* kraj, const char* nazev, int pocetPorizovacichStanic):
		kraj(kraj),
		nazev(nazev),
		porizovaciStanice("Porizovaci stanice",pocetPorizovacichStanic),
		zpracovatelskaStanice(),
		pocetPorizenychOkrsku(0)
	{
		pocetPrebiracichMist++;
	}
};

class PozadavekNaPorizeni : public Process {
public:
	
	Okrsek* okrsek;
	
	PozadavekNaPorizeni(Okrsek* okrsek):
		okrsek(okrsek)
	{}
	
	void Behavior();
};

class PozadavekNaZpracovani : public Process {
public:
	
	PrebiraciMisto* prebiraciMisto;
	
	PozadavekNaZpracovani(PrebiraciMisto* prebiraciMisto):
		prebiraciMisto(prebiraciMisto)
	{}
	
	void Behavior();
};

class PozadavekNaCentrum : public Process {
public:
	
	void Behavior();
};

#endif
