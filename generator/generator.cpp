#include "simlib.h"
#include <cmath>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <algorithm>


constexpr float pocetOkrskuKoeficient = 0.04;
constexpr float pocetVolicuOblastiKoeficient = 0.01;

constexpr int minOkrskuNaPM = 5;
constexpr int maxOkrskuNaPM = 20;

double NormalMinMax(double min, double max){
	double mi = max - min;
	double sigma = float(max - mi) / 6;
	return Normal(mi, sigma);
}

int main(){
	std::ifstream f("vstup.csv");
	std::ofstream ff("vystup.csv");
	if(!f.good()) throw std::runtime_error("Vstupni soubor se nepodarilo otevrit!");
	
	// oblast = kraj bez krajskeho mesta nebo krajske mesto
	std::string oblast;
	int konecnaSumaVolicu = 0;
	int konecnaSumaKomisaru = 0;
	int konecnyPocetOkrsku = 0;
	int prebiraciMisto = 0;
	int prebiraciIter;
	
	while(f.good()){
		
		std::string sloupecOblast;
		getline(f, sloupecOblast, '\t');
		
		std::string sloupecOkrsku;
		getline(f, sloupecOkrsku, '\t');
		int pocetOkrsku = atoi(sloupecOkrsku.c_str()) * pocetOkrskuKoeficient;
		
		std::string sloupecVolicu;
		getline(f, sloupecVolicu, '\n');
		int pocetVolicu = atoi(sloupecVolicu.c_str()) * pocetVolicuOblastiKoeficient;
		
		if(!f.good()) break;
		
		int sumaVygenerovanychVolicu = 0;
		std::vector<int> vygenerovaniVolici;
		
		// prvni pruchod - rozhozeni vsech volicu oblasti mezi okrsky
		for(int cisloOkrsku=0; cisloOkrsku < pocetOkrsku; cisloOkrsku++){
			
			// Pocet volicu v okrsku - prvotni pred korekci pro spravnou sumu
			int volicuOkrsku = 0;
			while(volicuOkrsku <= 0){
				volicuOkrsku = Exponential(100); // prvotni pocet volicu, jen pomerovy
			}
			vygenerovaniVolici.push_back(volicuOkrsku);
			sumaVygenerovanychVolicu += volicuOkrsku;
		}
		
		// zacatek PM (nemohou byt napric oblastmi)
		prebiraciIter = 0; // cislo okrsku v ramci PM
		prebiraciMisto++;
		
		// druhy pruchod okrsky
		int cisloOkrsku = 0;
		for(std::vector<int>::iterator it = vygenerovaniVolici.begin(); it != vygenerovaniVolici.end(); ++it, ++cisloOkrsku){
			
			konecnyPocetOkrsku++;
			
			int volicuOkrsku = (double) *it / sumaVygenerovanychVolicu * pocetVolicu; // konecny pocet volicu tohoto okrsku
			int zbyvaOkrsku = pocetOkrsku - cisloOkrsku; // pocet okrsku zbyvajicich do konce kraje
			konecnaSumaVolicu += volicuOkrsku;
			
			// pocet komisaru
			int minKomisaruOkrsku = volicuOkrsku <= 300 ? 4 : 5;
			float komisaruOkrsku = 0;
			while(komisaruOkrsku < minKomisaruOkrsku) komisaruOkrsku = NormalMinMax(minKomisaruOkrsku, minKomisaruOkrsku+6); // minKomisaruOkrsku - minKomisaruOkrsku+6
			komisaruOkrsku /= pocetOkrskuKoeficient; // prizpusobit koeficientu agregace okrsku
			komisaruOkrsku *= pocetVolicuOblastiKoeficient; // prizpusobit agregaci volicu
			komisaruOkrsku = round(komisaruOkrsku);
			konecnaSumaKomisaru += komisaruOkrsku;
			
			// PM se bude delit jen je-li dost okrsku na vytvoreni dalsiho PM
			if(zbyvaOkrsku >= minOkrskuNaPM){
				if(prebiraciIter == 0){
					prebiraciIter = NormalMinMax(minOkrskuNaPM, maxOkrskuNaPM); // stanoveni poctu okrsku noveho PM
					prebiraciMisto++;
				}
				prebiraciIter--;
			}
			
			int porizovacichStanic = 0;
			while(porizovacichStanic<2) porizovacichStanic = Normal(2,5);
			
			ff << sloupecOblast << '\t' << prebiraciMisto << '\t' << (int)porizovacichStanic << '\t' << volicuOkrsku << '\t' << (int)komisaruOkrsku << std::endl;
		}
	}
	
	ff.close();
	f.close();
	
	std::cout << "vygenerovano " << konecnaSumaVolicu << " volicu, " << konecnaSumaKomisaru << " komisaru, ";
	std::cout << konecnyPocetOkrsku << " okrsku a " << prebiraciMisto << " prebiracich mist" << std::endl;
}

