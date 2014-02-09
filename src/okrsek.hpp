#ifndef OKRSEK_HPP
#define OKRSEK_HPP

#include "simlib.h"
#include "sit.hpp"
#include <iostream>
#include <vector>

class Okrsek {
public:
	
	const char *nazev;
	int pocetVolicu;
	int naplanovanychPrichodu;
	int vUrne;
	int timeUzavreni;
	bool uzavrena = true;
	
	PrebiraciMisto* prebiraciMisto;
	std::vector<Facility*> komisari;
	std::vector<int> celkovyCasPraceKomisare;
	Facility plenta;
	Facility urna;
	UzavreniMistnosti* uzavreni;
	
	Okrsek(PrebiraciMisto* prebiraciMisto, const char* nazev, int pocetVolicu, int pocetKomisaru):
		prebiraciMisto(prebiraciMisto),
		nazev(nazev),
		pocetVolicu(pocetVolicu),
		naplanovanychPrichodu(0),
		vUrne(0),
		plenta("Plenta"),
		urna("Urna")
	{
		for(int i=0; i<pocetKomisaru; i++){
			komisari.push_back(new Facility("Komisar"));
			celkovyCasPraceKomisare.push_back(0);
		}
		celkovyPocetOkrsku++;
	}
	
	bool vsichniPrisli(){
		return naplanovanychPrichodu >= pocetVolicu;
	}
	
	bool vsichniHlasovali(){
		return vUrne >= pocetVolicu;
	}
	
	void init();
};

class PrichodVolice : public Event {
public:
	
	Okrsek* okrsek;
	
	PrichodVolice(Okrsek* okrsek):
		okrsek(okrsek)
	{}
	
	void Behavior();
};

class Volic : public Process {
public:
	Okrsek* okrsek;
	
	Volic(Okrsek* okrsek):
		okrsek(okrsek)
	{}
	
	void Behavior();
};

class OtevreniMistnosti : public Event {
public:
	Okrsek* okrsek;
	int delka;
	bool posledni;
	
	OtevreniMistnosti(Okrsek* okrsek, int delka, bool posledni):
		okrsek(okrsek),
		delka(delka),
		posledni(posledni)
	{}
	
	void Behavior();
};

// neni udalost ale proces, aby mohlo obsadit fronty
class UzavreniMistnosti : public Process {
public:
	Okrsek* okrsek;
	bool posledni;
	
	UzavreniMistnosti(Okrsek* okrsek, bool posledni):
		okrsek(okrsek),
		posledni(posledni)
	{}
	
	void Behavior();
};

class Scitani : public Process {
public:
	Okrsek* okrsek;
	
	Scitani(Okrsek* okrsek):
		okrsek(okrsek)
	{}
	
	void Behavior();
};

#endif
