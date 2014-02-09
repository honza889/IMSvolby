Okruh 9: Model státní volební infrastruktury
********************************************

Státní volební infrastruktura nechť se skládá z volebního informačního centra
a sítě volebních okrsků. Centrum přijímá zprávy od volebních okrsků a prezentuje
výsledky v jednotlivých krajích (nutno modelovat server centra jako obslužnou linku).

Volební okrsky jsou SHO obsahující obslužné linky: komise a místo pro provedení
volby do obálky ("za plentou"). Modelujte proces příchodů voličů v průběhu doby voleb.

Volební komise skončí práci buď po odvolení všech občanů v okrsku nebo okamžikem
konce volebního víkendu. Potom sčítá hlasy (doba je závislá na počtu obálek v urně)
a odesílá výsledky do centra.

Prostudujte systém voleb v ČR a síť volebních okrsků. Konkrétní síť okrsků generujte
náhodně s následujícím omezením: sumární počet voličů v krajích musí odpovídat realitě
a počet voličů v krajských městech musí odpovídat realitě. Okrsky nějak vhodně
agregujte tak, aby jejich celkový počet byl cca 200. Náhodně generovanou síť okrsků
uložte do souboru a experimenty provádějte stále nad stejným modelem sítě okrsků.

Zdokumentujte model sítě okrsků. Na experimentech ukažte propustnost centra, doby čekání
okrsků na připojení do centra, celkovou dobu práce lidí okrskových komisích.


Termíny
*******
[http://www.mvcr.cz/clanek/volby-do-poslanecke-snemovny-parlamentu-cr-246069.aspx]

* Volební okrsky
** program pro okrskové komise [http://www.volby.cz/cz/infookrsky.htm]
* Přebírací místa
** pořizovací software
** zpracovatelský software


Druhá celopološná zkouška na evropské volby
*******************************************
[http://www.czso.cz/csu/tz.nsf/i/druha_celoplosna_zkouska_na_evropske_volby]

V souladu s harmonogramem příprav na zpracování výsledků voleb do Evropského parlamentu dnes pracovníci Českého statistického úřadu úspěšně provedli druhou celoplošnou zkoušku sběru, přenosu a sčítání hlasů. V jejím průběhu se nevyskytl žádný vážnější problém. 

Předem zadané příklady simulující zápisy okrskových volebních komisí bezchybně došly prostřednictvím chráněné počítačové sítě ze všech 505 přebíracích míst do volebního zpracovatelského sálu v ČSÚ. Zkouška prověřila i schopnost zvládnout možné krizové situace, například poruchu programového vybavení a potřebu jeho opakované instalace. Test také ukázal, že zpracovatelský systém zvládne přísun velkého množství dat v jednom okamžiku. Je schopen v jedné minutě přenést, zkontrolovat, uložit a prezentovat data z více než 1200 okrsků. Teoreticky to znamená, že rychlost kompletního zpracování výsledků voleb by ze strany ČSÚ nepřesáhla 13 minut. 

ČSÚ simuloval i plnou zátěž prezentačního systému, který bude k dispozici na adrese www.volby.cz. Jeho prostřednictvím bude možné v neděli 13. června od 22. hodiny sledovat on-line zpracování průběžných výsledků. Volby do Evropského parlamentu budou v pátek 11. a v sobotu 12. června 2004. 

Roman Prorok
tiskový mluvčí předsedy ČSÚ 

