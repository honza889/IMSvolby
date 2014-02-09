#include "simlib.h"
#include "global.hpp"
#include "okrsek.hpp"
#include "sit.hpp"
#include <cmath>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <algorithm>

std::vector<Kraj*> kraje;

Histogram dobaMeziPrichodyOkrsky("Doba mezi prichody volicu na okrsek",0,120,20);
Histogram volebniUcastOkrsky("Volebni ucast po okrscich (%)",0,5,21);
Histogram dobaPraceKomiseOverovani("Prumerna doba prace komisare - overovani",0,250,20);
Histogram dobaPraceKomiseScitani("Doba prace komise - scitani",960,480,20);
Histogram dobaCekaniNaPorizeni("Doba kterou stravi komise na PM",350,50,14);
Histogram dobaCekaniNaCentrum("Doba cekani na centrum",0.0,0.1,15);
Histogram dobaPozadavku("Doba cesty a vyrizovani pozadavku na centrum",2.09,0.04,15);

bool nakratko = true;
int dobaScitani = 0;

void initOkrsky(){
	for(Kraj* kraj : kraje){
		for(PrebiraciMisto* prebiraciMisto : kraj->prebiraciMista){
			for(Okrsek* okrsek : prebiraciMisto->okrsky){
				okrsek->init();
			}
		}
	}
}

void loadCsv(char *csvFile){
	int kraju = 0, prebiracichMist = 0, okrsku = 0;
	std::ifstream f(csvFile);
	if(!f.good()) throw std::runtime_error("Soubor s okrsky se nepodarilo otevrit!");
	
	std::string aktualniKraj;
	std::string aktualniPM;
	Kraj* kraj;
	PrebiraciMisto* misto;
	
	while(f.good()){
		std::string sloupecKraj;
		getline(f, sloupecKraj, '\t');
		std::string sloupecPM;
		getline(f, sloupecPM, '\t');
		std::string sloupecPocetvPM;
		getline(f, sloupecPocetvPM, '\t');
		std::string sloupecVolicu;
		getline(f, sloupecVolicu, '\t');
		std::string sloupecKomisaru;
		getline(f, sloupecKomisaru, '\n');
		if(!f.good()) break;
		
		// novy kraj
		if(aktualniKraj.compare(sloupecKraj)!=0){
			aktualniKraj = sloupecKraj;
			kraj = new Kraj(aktualniKraj.c_str());
			kraje.push_back(kraj);
			kraju++;
		}
		
		// nove prebiraci misto
		if(aktualniPM.compare(sloupecPM)!=0){
			aktualniPM = sloupecPM;
			misto = new PrebiraciMisto(kraj, sloupecPM.c_str(), atoi(sloupecPocetvPM.c_str()));
			kraj->prebiraciMista.push_back(misto);
			prebiracichMist++;
		}
		
		// nove okrsky
		int pocetVolicuVokrseku = atoi(sloupecVolicu.c_str());
		int pocetKomisaru = atoi(sloupecKomisaru.c_str());
		
		Okrsek* okrsek = new Okrsek(misto,"Okrsek",pocetVolicuVokrseku,pocetKomisaru);
		misto->okrsky.push_back(okrsek);
		okrsku++;
	}
	f.close();
	std::cout << "Nacteno " << kraju << " kraju, " << prebiracichMist << " prebiracich mist a " << okrsku << " okrsku." << std::endl;
}

int main(int argc, char** argv){ // popis experimentu
	if(argc<3) throw std::invalid_argument("Nutno zadat jako parametr adresu CSV s okrsky a vystupniho souboru");
	nakratko = (argc==3);
	
	//DebugON();
	
	dobaMeziPrichodyOkrsky.Clear();
	volebniUcastOkrsky.Clear();
	dobaPraceKomiseOverovani.Clear();
	dobaCekaniNaCentrum.Clear();
	
	SetOutput(argv[2]);
	Init(0, 48*60*60);
	
	loadCsv(argv[1]);
	initOkrsky();
	
	std::cout << "Simulace zahajena" << std::endl;
	Run();
	std::cout << "Simulace dokoncena" << std::endl;
	
	simlib3::Print("Doba scitani: %d\n",dobaScitani);
	simlib3::Print("Propustnost serveru: %lf\n",serverCentra.tstat.Sum()/serverCentra.tstat.Number());
	//dobaMeziPrichodyOkrsky.Output();
	volebniUcastOkrsky.Output();
	dobaPraceKomiseOverovani.Output();
	dobaPraceKomiseScitani.Output();
	dobaCekaniNaPorizeni.Output();
	dobaCekaniNaCentrum.Output();
	dobaPozadavku.Output();
}

