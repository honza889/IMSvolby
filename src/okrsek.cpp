#include "simlib.h"
#include "global.hpp"
#include "okrsek.hpp"
#include <iostream>
#include <vector>

int celkovyPocetOkrsku = 0; // inkrementuje konstruktor okrsku
int planovanyKonecZacatkuScitani;

void Okrsek::init(){
	// naplanovani volebniho vikendu
	(new OtevreniMistnosti(this,28800,false))->Activate(Time); // Patek 14:00 - 22:00
	(new OtevreniMistnosti(this,21600,true))->Activate(Time+64800); // Sobota 8:00 - 14:00
}

void OtevreniMistnosti::Behavior(){
	
	okrsek->uzavrena = false;
	okrsek->timeUzavreni = Time + delka;
	if(posledni) planovanyKonecZacatkuScitani = okrsek->timeUzavreni;
	
	// naplanovani prichodu volicu (prvni prichazi po 5 minutach)
	okrsek->naplanovanychPrichodu++;
	PrichodVolice* pv = new PrichodVolice(okrsek);
	pv->Activate(Time + Exponential(10*60));
	
	// naplanovani uzavreni mistnosti
	okrsek->uzavreni = new UzavreniMistnosti(okrsek,posledni);
	okrsek->uzavreni->Activate(okrsek->timeUzavreni);
}

void PrichodVolice::Behavior(){
	//std::cout << "prichod" << std::endl;
	
	// dalsi prichazi zhruba za 5,99 minut
	double dalsi = Exponential(5.99*60); // 5.99 minut = prumerna ucast 59%
	dobaMeziPrichodyOkrsky(dalsi);
	
	// ale jen pokud nebude po zaviraci dobe a nebude nadpocetny
	if(Time + dalsi < okrsek->timeUzavreni && okrsek->naplanovanychPrichodu+1 < okrsek->pocetVolicu){
		okrsek->naplanovanychPrichodu++;
		Activate(Time + dalsi);
	}
	
	(new Volic(okrsek))->Activate();
}

void Volic::Behavior(){
	double prichod = Time;
	
	// volba komisare - nevybira volic - rovnomerne napr. podle ulic
	int komisarVolice = Random()*okrsek->komisari.size();
	
	// kontrola obcanky komisarem 15-45s
	Seize(*okrsek->komisari[komisarVolice]);
	int zacatekObsluhyKomisarem = Time; // zacatek prace komisare
	Wait(NormalMinMax(15, 45));
	okrsek->celkovyCasPraceKomisare[komisarVolice] += Time - zacatekObsluhyKomisarem; // konec prace komisare
	Release(*okrsek->komisari[komisarVolice]);
	
	// strceni hlasu do obalky 10-80s
	Seize(okrsek->plenta);
	Wait(NormalMinMax(10, 80));
	Release(okrsek->plenta);
	
	// vhozeni do urny 3-20s
	Seize(okrsek->urna);
	Wait(NormalMinMax(3, 20));
	okrsek->vUrne++;
	Release(okrsek->urna);
	
	// zahajeni uzavirani mistnosti pri odhlasovani posledniho volice
	if(okrsek->naplanovanychPrichodu == okrsek->pocetVolicu && !okrsek->uzavrena){
		okrsek->uzavreni->posledni = true;
		okrsek->uzavreni->Activate(); // prepsat predchozi naplanovani na nove (odstrani prechozi nacasovani)
	}
}

void UzavreniMistnosti::Behavior(){
	if(posledni && !okrsek->uzavrena){ // posledni uzavreni
		
		// dalsi volici uz nesmi prichazet
		okrsek->uzavrena = true;
		
		// vyckani na vyprazdneni front
		for(int i=0; i<okrsek->komisari.size(); i++){
			Seize(*okrsek->komisari[i]);
			dobaPraceKomiseOverovani(okrsek->celkovyCasPraceKomisare[i]);
		}
		Seize(okrsek->plenta);
		Seize(okrsek->urna);
		
		// uzavirani dokonceno, zahajeni scitani
		(new Scitani(okrsek))->Activate();
	}
}

void Scitani::Behavior(){
	
	double zacatekScitani = Time;
	
	// vysypani urny, priprava scitani
	Wait(NormalMinMax(2*60,10*60));
	
	// pocitani hlasu zavisle na poctu hlasu
	Wait(okrsek->vUrne * NormalMinMax(10,60));
	
	// doba prepisu do okrskoveho pocitace
	Wait(NormalMinMax(15*60,30*60));
	
	// zapocitani volebni ucasti
	volebniUcastOkrsky(okrsek->vUrne*100/okrsek->pocetVolicu);
	std::cout << "Ucast v okrsku je " << okrsek->vUrne << "/" << okrsek->pocetVolicu << std::endl;
	
	dobaPraceKomiseScitani(Time - zacatekScitani);
	
	if(nakratko){
		// odeslani souboru s vysledky primo okrskovym pocitacem
		(new PozadavekNaCentrum())->Activate();
		std::cout << "Okrsek odeslan do centra" << std::endl;
	}else{
		// doruceni souboru s vysledky (na flash pameti) zabere okolo 15-60 minut, nasleduje porizeni
		(new PozadavekNaPorizeni(okrsek))->Activate(Time + NormalMinMax(30*60,90*60));
		std::cout << "Okrsek odeslan na prebiraci misto" << std::endl;
	}
}

