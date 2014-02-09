Okruh 9: Model státní volební infrastruktury

Státní volební infrastruktura nechť se skládá z volebního informačního centra a sítě volebních okrsků. Centrum přijímá zprávy od volebních okrsků a prezentuje výsledky v jednotlivých krajích (nutno modelovat server centra jako obslužnou linku). Volební okrsky jsou SHO obsahující obslužné linky: komise a místo pro provedení volby do obálky ("za plentou"). Modelujte proces příchodů voličů v průběhu doby voleb. Volební komise skončí práci buď po odvolení všech občanů v okrsku nebo okamžikem konce volebního víkendu. Potom sčítá hlasy (doba je závislá na počtu obálek v urně) a odesílá výsledky do centra. Prostudujte systém voleb v ČR a síť volebních okrsků. Konkrétní síť okrsků generujte náhodně s následujícím omezením: sumární počet voličů v krajích musí odpovídat realitě a počet voličů v krajských městech musí odpovídat realitě. Okrsky nějak vhodně agregujte tak, aby jejich celkový počet byl cca 200. Náhodně generovanou síť okrsků uložte do souboru a experimenty provádějte stále nad stejným modelem sítě okrsků. Zdokumentujte model sítě okrsků. Na experimentech ukažte propustnost centra, doby čekání okrsků na připojení do centra, celkovou dobu práce lidí okrskových komisích.

### Vstupní data: ###
* Okrsků 200 v každém kraji (celkem 5200)
* Počet voličů v každém kraji A krajském městě musí odpovídat realitě
* Použít místo toho reálná data je přípustné

### Výstupní data: ###
* Propustnost centra (= počet vyřízených požadavků za jednotku času)
* Doby čekání okrsků na připojení do centra
* Celkovou dobu práce lidí okrskových komisí
 * suma obsluhy všech voličů na komisaře
 * doba sčítání

