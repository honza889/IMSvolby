#include "global.hpp"
#include "sit.hpp"
#include "okrsek.hpp"
#include <limits>

int pocetPrebiracichMist = 0; // inkrementuje konstruktor prebiraciho mista
int prijato = 0;

Facility vpn("VPN");
Facility serverCentra("Server centra");

void PozadavekNaPorizeni::Behavior(){
	
	// pozadavky na porizeni (flashky v rukou komisaru) vyckaji
	// na porizeni u jedne z porizovacich stanic (spolecna fronta)
	
	double zacatekPorizovani = Time;
	
	Enter(okrsek->prebiraciMisto->porizovaciStanice,1);
	Wait(NormalMinMax(2*60, 10*60)); // nakopirovani z flashky do programu a sepsani protokolu trva 2 - 10 minut
	okrsek->prebiraciMisto->pocetPorizenychOkrsku++;
	Leave(okrsek->prebiraciMisto->porizovaciStanice,1);
	
	dobaCekaniNaPorizeni(Time - zacatekPorizovani);
	
	// po vyrizeni vsech pozadavku PM
	if(okrsek->prebiraciMisto->pocetPorizenychOkrsku == okrsek->prebiraciMisto->okrsky.size()){
		// preneseni na flash pamet z posledni porizovaci stanice trva 5-10 minut
		(new PozadavekNaZpracovani(okrsek->prebiraciMisto))->Activate(Time + NormalMinMax(5*60,10*60));
	}
}

void PozadavekNaZpracovani::Behavior(){
	
	// Zpracovani - prenos dat z porizovacich stanic na zpracovatelskou stanici
	// rovnez se provadi skrze flash pamet (zpracovatelska stanice nesmi byt pripojena do jine site)
	Seize(prebiraciMisto->zpracovatelskaStanice);
	// prenost trva [pocet flashek]*15sekund + organizacni minutu AZ [pocet flashek] minut + organizacnich 5 minut
	for(int i=0; i<prebiraciMisto->porizovaciStanice.Capacity(); i++){
		Wait(NormalMinMax(15,60));
	}
	Wait(NormalMinMax(1*60,5*60));
	Release(prebiraciMisto->zpracovatelskaStanice);
	
	(new PozadavekNaCentrum())->Activate();
}

void PozadavekNaCentrum::Behavior(){
	
	// Jelikoz se presne parametry volebniho serveru CSU nepodarilo zjistit,
	// je predpokladan jediny iterativni server neschopny soubezneho zpracovani vice pozadavku
	// s frontou pozadavku ve forme Socketu operacniho systemu serveru
	
	double zacatekPozadavku = Time;
	
	Wait(NormalMinMax(0.025,0.25)); // cesta po vetvi VPN (nesdilena)
	Seize(*vpn);
	Wait(NormalMinMax(0.025,0.25)); // cesta po uzke casti VPN (k serveru)
	Release(*vpn);
	
	double zacatekCekaniNaCentrum = Time;
	Seize(*serverCentra); // cekani na centrum
	dobaCekaniNaCentrum(Time - zacatekCekaniNaCentrum);
	// komunikace se serverem trva 0.2 - 2 sekundy
	Wait(NormalMinMax(0.2,2.0));
	prijato++;
	Release(*serverCentra);
	
	dobaPozadavku(Time - zacatekPozadavku);
	
	if(nakratko){
		if(prijato == celkovyPocetOkrsku){
			dobaScitani = Time - planovanyKonecZacatkuScitani;
			std::cout << "Volby dokonceny - scitani kompletni (po okrscich trvalo " << dobaScitani << ")" << std::endl;
		}
	}else{
		if(prijato == pocetPrebiracichMist){
			dobaScitani = Time - planovanyKonecZacatkuScitani;
			std::cout << "Volby dokonceny - scitani kompletni (po PM trvalo " << dobaScitani << ")" << std::endl;
		}
	}
}

