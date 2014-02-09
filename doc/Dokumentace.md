## 1. Úvod ##

V této práce je řešena implementace státní volební infrastruktury, která bude použita pro sestavení modelu státní volební infrastruktury.

Na základě modelu a simulačních experimentů bude ukázáno chování volebního systému v současné podobě, a v podobě kdy jsou volební výsledky odesílány z okrskových pracovišť přímo na server Českého statistického úřadu (ČSÚ).

Smyslem experimentů je zjistit, nakolik by bylo možné snížit čas sčítání volebních výsledků odstraněním přebíracích míst, tedy pracovišť shromažďujících volební výsledky z okolních okrsků a odesílajících tyto výsledky na server ČSÚ.

Pro zpracování modelu bylo nutné v terénu nastudovat mimo volebního zákona také v praxi používané postupy volebních komisí a pracovníků veřejné správy zasahujících do procesu sběru nebo sčítání hlasů. Mnoho informací o infrastruktuře používané pro přenos výsledků voleb za jednotlivé okrsky bylo ale mnohem hůře dostupných - bylo nutné informace o celku sbírat ze střípků technických detailů obsažených v přílohách zadání veřejných zakázek souvisejících právě s volební infrastrukturou. Mimo praktických informací popsaných v této zprávě tak bylo nutné projít velké množství informačních zdrojů, z nichž většina neobsahovala žádné informace potřebné pro implementaci modelu.

### 1.1 Autoři práce ###

Autory této práce jsou Jan Kalina (xkalin03@stud.fit.vutbr.cz) a Jan Doležal (xdolez52@stud.fit.vutbr.cz). 
Práce byla konzultována s technikem přebíracího místa v Hustopečích, Gustavem Mrázkem.

### 1.2 Validita projektu ###

Vstupními daty modelu je seznam oblastí (oblastí je krajské město nebo kraj bez krajského města) včetně počtu okrsků a voličů v každé oblasti. Tato data byla získána z veřejně dostupných dat na serveru OtevřenéVolby.cz[1](http://otevrenevolby.cz/data/psp_2013_obce-zahranici.csv).

Četnost příchodu voličů byla v modelu stanovena za pomoci exponenciální pravděpodobnostní funkce tak, aby celková volební účast odpovídala realitě. Reálná volební účast je dle serveru Volby.cz[2](http://www.volby.cz/pls/ps2013/ps2?xjazyk=CZ) 59,48%. Metodou půlení intervalu tak byl střed pravděpodobnostní funkce určen na 5,99 minuty, čímž bylo reálné volební účasti dosaženo s přesností na 0,17% - volební účast v simulačním modelu činila 59.51% pro první variantu (s přebíracími místy) a 59.65% pro druhou variantu (bez přebíracích míst).

(V jakém prostředí a za jakých podmínek probíhalo experimentální ověřování validity modelu - doplní se až nějaké slušné výsledky budou)

## 2. Rozbor tématu a použitých metod/technologií ##

Před zahájením voleb má každá okrsková komise k dispozici seznam registrovaných voličů. Volební místnost je otevřena po dvě časová okna - první volební den od 14:00 do 22:00, druhý volební den od 8:00 do 14:00. [§1 odst. 4 č. 247/1995 Sb.](http://www.nssoud.cz/zakony/247_1995.pdf) Voliči přicházejí do volebních místností v nichž jsou registrování v těchto časových oknech, v intervalech které je možné aproximovat exponenciální pravděpodobností jejíž střed je možné určit půlením intervalu a porovnáváním volební účasti z modelu s realitou. (viz. Validita projektu)

Minimální počet volebních komisařů v každém okrsku je stanoven zákonem na 5 pro okrsky nad 300 voličů a 4 pro méně, přičemž tento počet může být navýšen rozhodnutím místního starosty. [§14c odst. C č. 247/1995 Sb.](http://www.nssoud.cz/zakony/247_1995.pdf) Jednotliví komisaři obvykle mívají voliče striktně rozděleny a před každým se tak tvoří samostatná fronta. Doba kterou trvá volebnímu komisaři obsloužení voliče (tedy nalezení voliče v seznamu, jeho odškrtnutí a vydání oficiální obálky) byla pro potřeby modelu experimentálně zjištěna jako odpovídající normálnímu rozdělení pravděpodobnosti v intervalu 15 - 45 sekund.

Obsloužení voliči postupují do fronty na plentu, za kterou vkládají hlasovací lístek do obálky. Tento úkon jim dle měření trvá 10 - 80 sekund. Nakonec přistupují k volební urně (opět jediná fronta), kde (dle měření) 3 - 20 sekund vhazují obálku do urny a odchází.

Hlasování je ukončeno druhý den voleb 14:00 nebo poté co odvolí všichni voliči. Jsou-li v čas plánovaného ukončení voleb ve volební místnosti přítomni voliči, je jim dovoleno odvolit, ale dalším voličům je již zabráněno ve vstupu do volební místnosti. Po ukončení hlasování (kdy volební místnost opustí zbývající voliči) je zahájeno sčítání.

Sčítání se skládá z přípravné části, sčítání jednotlivých hlasů a přepisu sečtených hodnot do okrskového počítače. Přípravná část zahrnuje rozpečetění urny a připravení potřebných tiskopisů. To trvá dle měření 2 - 10 minut. Samotné sčítání spočívá v započítávání jednotlivých hlasů. Započtení jedné volební obálky dle měření trvá 10 - 60 sekund. Závěrečné sečtení hlasů včetně přepisu počtu hlasů do okrskového počítače a vyexportování na flash paměť pak 15 - 30 minut.

Protokol a flash paměť s výsledkem sčítání jsou neseny na přebírací místo do kterého okrsek spadá. Přebíracích míst je v ČR dle měsíčníku ČSÚ okolo 500. (strana 5, ročník 3, 09/2013) Tato cesta je běžně 0-20 km dlouhá, při normálním pravděpodobnostním rozložení. (Tento údaj byl získán ze vzorku 10 náhodně vybraných obcí, ze seznamu přebíracích míst a obcí, jejihž okrsky pod tyto přebírací místa spadají a odhadu délky cesty dle služby Mapy.cz) Tato cesta trvá běžně 1 - 30 minut. (Minimální hodnota vychází z předpokladu že se volební okrsková komise nachází ve stejné budově jako přebírací místo, maximální hodnota pak z odhadu doby cesty ze služby Mapy.cz)

Komise jsou na přebíracích místech odbavovány na jedné ze pořizovacích stanic. Odbavení komise spočívá v naimporotování dat z flash paměti do pořizovací stanice a sepsaní protokolu. Protože pořizovací místa nejsou veřejnosti přístupná, nebylo možné dobu pořizování přesmě změřit. Obsluhou byla ale doba pořízení odhadnuta na 5 - 10 minut. Pořizovací stanice tedy postupně shromažďují výsledky z jednotlivých okrsků, dokud nejsou odbaveny všechny komise.

Následně jsou takto shromážděná data skrze flash paměť nakopírována do zpracovatelské stanice, což je počítač izolovaný od běžné počítačové sítě a připojen do virtuální privátní sítě (VPN). Ačkoli termín VPN obvykle označuje virtuální síť postavenou za pomoci kryptografie nad běžnou počítačovou sítí, v tomto případě je takto označována virtuální LAN (VLAN) - virtuální síť vytvořená hardwarovými síťovými prvky nad síťovou infrastrukturou, v tomto případě společnosti O2.

Zpracovatelská stanice předává data skrze VPN na hlavní databázový server ČSÚ, ze kterého se data dále replikují na ostatní databázové servery, serery náležející prezentačním serverům webu Volby.cz a dedikovaný server pro média. Pro potřeby našeho modelu budeme data považovat za sečtená ve chvíli, kdy budou veškterá data dostupná na hlavním databázovém serveru.

Minimální rychlost VPN je zadáním veřejné zakázky[1](http://ezak-czso.cz/document_906/77524ea3d11ecb7-P%C5%99%C3%ADloha%20%C4%8D.%201A_el%20podpis.pdf) stanovena na 100 kb/s oběma směry a dostupnost na 99,0%. Velikost dat odesílaných ze zpracovatelské stanice byla obsluhou stanice odhadnuta na jednotky kilobytů. Není již ale bohužel stanovena latence, proto ani čas přenosu dat na hlavní databázový server nemůže být přesně vypočítán. Doba navázání spojení s tímto serverem a přenosu dat je tak na základě měření doby přenosu na podobně dimenzovaný server (eva.fit.vutbr.cz) odhadnuta na 0,5 - 2 sekundy. Doba přenosu počítačovou sítí je určena odečtením času potřebného k přenosu stejných dat v rámci lokální počítačové sítě od času potřebného pro přenesení těchto dat v rámci sítě internet. Předpokládá se přitom že spojení mezi dvěma klienty sítě O2 bude maximálně tak časově náročné jako spojení z počítače připojeného k internetu skrze síť O2 se službou v síti internet. Takto byla doba přenosu paketu z aplikace po síti VPN do schránky serveru spočtena jako 0,05 - 0,5 sekundy.

#Síť VPN spojující zpracovatelské stanice s hlavním databázovým serverem je modelována zjednodušeně jako jedno zařízení s výlučným přístupem a samotný databázový server jako zařízení druhé. Na straně serveru se předpokládá zamykání databázových tabulek v průběhu zpracování jednoho požadavku. Na straně sítě VPN se předpokládá že požadavek o velikosti jednotek kilobytů je přenášen v rámci jediného paketu a pro zjednodušení se síť VPN modeluje jako jediná linka, což není tolik abstrahující jak by se na první pohled mohlo zdát, neboť právě jediná linka je nejužším místem sítě a to linka kterou je připojený hlavní databázový server. Abstrahovali jsme tak jen a pouze od bufferů síťových prvků VPN.

### 2.1 Popis použitých postupů pro vytvoření modelu ###

Pro implementaci simulačního modelu byl zvolen jazyk C++ verze 11 a simulační knihovna Simlib. Knihovna Simlib byla zvolena, protože poskytuje veškerou funkcionalitu potřebnou pro implementaci modelu a je vydána pod licencí GNU LGPL. Provádění experimentů ani jejich opakování tak nevyžaduje palatbu žádných licenčních poplatků, narozdíl od komerčních prostředků, jakým je prostředí Dymola.

Seznam okrsků byl generován náhodně na základě dat o počtu voličů a počtu okrsků v jednotlivých krajích. Ze zdrojového CSV (získaného ze server OtevřenéVolby.cz[1](http://otevrenevolby.cz/data/psp_2013_obce-zahranici.csv), viz. Validita modelu) byla data naimportována do databáze MySQL nástrojem phpMyAdmin a dotazováním nad takto vzniklou databází byla získána agregovaná data - počet okrsků a voličů v jednotlivých krajích a v krajských městech. Za pomoci tabulkového procesoru LibreOffice Calc byla tato data přetvořena v seznam oblastí (oblastí je krajské město nebo kraj bez krajského města) s počet voličů a okrsků v jednotlivých oblastech. (Okrsky a voliče krajských měst bylo nutné odečíst od okrsků a voličů zbytku kraje.) Tento seznam oblastí by exportován do CSV (`generator/vstup.csv`).

Seznam okrsků byl z tohoto seznamu získán za pomoci generátoru vytvořeného stejně jako samotný model v jazyce C++ s knihovnou Simlib. Takto vzniklý CSV (`generator/vystup.csv`) je následně předáván samotnému modelu jako parametr příkazové řádky.

### 2.2 Popis původu použitých metod/technologií ###

Kompilátor jazyka C++ - g++, databázový software MySQL, nástroj pro jeho zprávu phpMyAdmin a tabulkový kalkulátor LibreOffice Calc byly získány ze standardních repozitářů distribuce Debian a jsou vydány pod licencí GPL.
Knihovna Simlib byla stažena z oficiálního webu knihovny (http://www.fit.vutbr.cz/~peringer/SIMLIB/) ve verzi 3.02 a jejím autorem je doktor Petr Peringer.
Model ani generátor nepoužívají jiných prostředků než jsou dostupné ze standardní knihovny jazyka C++ a z knihovny Simlib.

## 3. Koncepce ##

V této kapitole je definován konceptuální model vytvořený na základě známých faktů o systému zmíněných v předchozích kapitolách, na základě kterého byl vytvořen simulační model.

### 3.1 Způsob vyjádření modelu ###

Hlasování voličů je znázorněno petriho sítí na obrázku 1. Cesta kterou tečou sečtená data z jednotlivých okrsků na server je znázorněna diagramem datových toků (dataflow) na obrázku 2. pro první variantu (stávající způsob používaný ve volebním systému) a na 3. obrázku pro druhou variantu (navrhované zjednodušení volebního systému, jehož efektivita je v této práci ověřována). 

Ačkoli by se na první pohled mohlo zobrazení cesty dat z okrsků na server centra formou petriho sítě jevit jako vhodnější než použití diagramu datových toků, není tomu tak, neboť 

Diagram datových toků byl pro reprezentaci cesty dat z okrsků na server centra kvůli velkému množství přechodů podmíněných přítomností všech 

### 3.2 Formy konceptuálního modelu ###

TODO: zde budou "schémata části systému zodpovědné za doručení výsledků hlasování na server centra"

Obrázek 1: Petriho síť znázorňující hlasování voličů

Obrázek 2: Diagram datových toků sečtených dat pro 1. variantu řešení

Obrázek 3: Diagram datových toků sečtených dat pro 2. variantu řešení


Hlasování začíná otevřením volebních místností první volební den v 14:00, ve 22:00 hodin prvního volebního dne je přerušeno a opět obnoveno druhý volební den v 8:00. Ve 14:00 hodin druhého volebního dne je volební místnost uzavřena naposledy a následuje sčítání. V době kdy je volební místnost otevřená přicházejí jednotliví voliči. Čas mezi příchody dvou po sobě jdoucích voličů lze aproximovat za pomoci exponenciálního rozložení pravděpodobnosti.

Voliči kteří přijdou vstupují do fronty jednoho z volebních komisařů. Ti jsou v konceptuálním modelu modelováni jako obslužná zařízení, mezi kterými si volič náhodně vybírá. (Což je ekvivaletní přístupu, kdy jsou voliči komisařům předem přiřazeni, např. podle ulic trvalého bydliště) Obsluha komisařem trvá, jak již bylo řečeno, 15 - 45 sekund s normálním rozdělením pravděpodobnosti.

Obsloužení voliči vstupují do fronty na volební plentu, modelované jako obslužné zařízení. Za plentou voliči stráví 10 - 80 sekund, opět v normálním rozložení pravděpodobnosti. Zpoza plenty voliči vstupují do fronty na volební urnu, která je také modelovaná jako obslužné zařízení, ale voliči ji obsazují pouze 3 - 20 sekund a navýší přitom počet hlasů ve volební urně. Poté voliči systém opouští.

V čas kdy má být volební místnost uzavřena je zastaveno generování přicházejících voličů. (Model vychází z předpokladu, že voliči znají dobu kdy jsou volební místnosti otevřené a v době kdy ví že jsou zavřené nepřijdou čekat před volební místnost.) Odbavování jednotlivých voličů ale pokračuje až do vyprázdnění front ve volební místnosti.

Po závěrečném uzavření volební místnosti je zahájeno sčítání. Sčítání je tvořeno třemi fázemi - fází přípravnou (vysypání urny, tisk formulářů) jejíž trvání nezávisí na počtu hlasů, fází samotného sčítání, kdy jsou otvírány a započítávány jednotlivé obálky a jejíž trvání lineárně závisí na počtu hlasů, jež byly vhozeny do volební urny, a fází závěrečnou, kdy jsou formuláře vyplněny a výsledky zadány do okrskového počítače. Poté zástupci volební komise odnášejí výsledky na přebírací místo.

Přebírací místo je tvořeno skupinou paralelně pracujících obslužných zařízení se společnou frontou - pořizovacími pracovišti. Pořizovací pracoviště vyřizují požadavky na pořízení z jednotlivých okrsků. Po vyřízení požadavků všech okrsků jsou data z pořizovacích stanic přenesena na zpracovatelské pracoviště, odkud je skrze síť VPN odeslán požadavek s daty všech okrsků na hlavní volební server ČSÚ. Potom co hlavní volební server obdrží data o všech okrscích považujeme volby za dokončené.

## 4. Architektura simulačního modelu ##

Pro implementaci simulačního modelu byl zvolen jazyk C++ verze 11 a simulační knihovna Simlib. Knihovna Simlib byla zvolena protože poskytuje veškerou funkcionalitu potřebnou pro implementaci modelu.

### 4.1 Mapování abstraktního modelu do simulačního ###

Simulace začíná inicializací objektů okrsků (`Okrsek`) na základě dat ze souboru okrsků. Samotný okrsek je pouze datovým objektem sdružujícím objekty vztahující se k jednomu okrsku. Jeho jedinou významnou metodou je metoda `init()`, která naplánuje otevření volební místnosti na začátku každého dne voleb - událost `OtevreniMistnosti`. Otevření místnosti způsobí naplánování uzavření místnosti (událost `UzavreniMistnosti`) a první naplánování příchodu voliče (události `PrichodVolice`), která následně způsobuje plánování příchodů voličů až dokud plánovaný čas příchodu dalšího voliče nepřekročí čas uzavření volební místnosti, nebo počet vygenerovaných voličů nedosáhne počtu voličů v okrsku. Právě příchod voliče totiž způsobuje vytvoření procesu voliče (`Volic`) a jeho spuštění.

Volič po svém příchodu vstupuje do fronty jednoho z komisařů, který je modelován jako obslužné zařízení (`Okrsek.komisari[]`). Poté vstupuje do fronty na plentu, která je modelována jako obslužné zařízení `Okrsek.plenta` a nakonec do fronty na volební urnu (`Okrsek.urna`), poté systém opouští. Byl-li ale poslední, aktivuje přitom předčasně událost `UzavreniMistnosti`, přičemž ji označí jako poslední uzavření ve volebním víkendu. Namísto uzavření volební místnosti na konci dne tak je místnost uzavřena již při odchodu posledního voliče. Událost uzavření místnosti místnost označí jako uzavíranou, postupně obsadí všechny fronty místnosti v obdobném pořadí jako volič a tím vyčká na odvolení voličů ve frontách. Jakmile se uzavření dostane až k volební urně (a ve frontách tedy již nejsou žádní voliči), je zahájeho sčítání hlasů.

Sčítání je reprezentováno procesem `Scitani`, jeho trvání je určeno součtem času trvání všech tří fází, jehož způsob určení je definován v části Koncepce. Po dokončení sčítání zástupci volební komise odnášejí výsledky na přebírací místo - generují požadavky na pořízení (`PozadavekNaPorizeni`).

Na přebíracím místě se požadavky na pořízení řadí do společné fronty, ze které je odebírají pořizovací pracoviště. Ty jsou tedy modelovány jako sklad `PrebiraciMisto.porizovaciStanice`. Dokončením pořizování posledního požadavku na pořízení daného přebíracícho místa je vytvořen požadavek na zpracování zpracovatelským pracovištěm, které je také součástí přebíracího místa. Je modelováno jako zařízení `PrebiraciMisto.zpracovatelskaStanice`. Doba zpracování závisí na počtu pořizovacích pracovišť a po jeho dokončení je vytvořen požadavek na centrum (proces `PozadavekNaCentrum`), který nejprve obsadí síť VPN (zařízení `vpn`), vyčká ve schránce (socketu) na uvolnění serveru (zařízení `serverCentra`), který následně obsadí.

Druhá varianta řešení je implementována obdobně, jen proces `Scitani` namísto požadavku na pořízení generuje požadavek na centrum.

## 5. Podstata simulačních experimentů a jejich průběh ##

Cílem experimentů bylo změřit možné zefektivnění procesu sčítání odesíláním výsledků z okrskových počítačů přímo na server centra, namísto jejich nošení na přebírací místa. Konkrétně tedy o kolik by byla snížena celková doba sčítání a dále pak jak by se tímto krokem změnila propustnost serveru centra a doba čekání požadavků na server centra. Pro naplnění požadavků zadání pak bylo nutné měřit také celkovou dobu práce lidí v okrskových komisích.

### 5.1 Postup experimentování ###

Pro potřeby porovnání obou variant řešení distribuce volebních výsledků byl simulační model popsaný výše spuštěn pro obě varianty pro dvě různé střední hodnoty exponenciální pravděpodobnostní funkce určující příchody voličů - pro hodnotu jejíž důsledkem bude celková volební účast odpovídající reálné volební účasti a pro hodnotu jejíž důsledkem bude enormě vysoká volební účast. U všech 4 experimentů byla měřena doba sčítání hlasů (od nejzasšího termínu uzavření volebních místností po přijetí všech výsledků serverem centra), volební účast, propustnost centra, doba čekání požadavků na server centra a pro úplné splnění zadání také celková doba práce lidí v okrskových komisích. Jejich srovnáním doufáme nalezneme řešení.

### 5.2 Dokumentace jednotlivých experimentů ###

#### 5.2.1 Experimenty s reálnou volební účastí ####

Tato podkapitola porovnává výsledky experimentů prováděné na simulačním modelu v obou variantách (s přebíracími místy i bez nich) při střední hodnotě exponenciální pravděpodobnostní funkce určující četnost příchodů voličů 5,99 minuty, což je experimentálně metodou půlení intervalu určená hodnota s cílem dosáhnout volební účasti odpovídající realitě - 59,48%. (způsob určení viz. Validita projektu)

(zde grafy a jejich popis)

#### 5.2.2 Experimenty s enormní volební účastí ####

(totéž jen s jinou střední hodnotou a volební účastí)

### 5.3 Závěry experimentů ###

Byly provedeny celkem 4 experimenty - pro varianty s a bez přebíracích míst a pro střední hodnotu exponenciální pravděpodobnostní funkce určující čas příchodu dalšího voliče odpovídající reálné volební účasti a enormní volební účasti. Z experimentů lze odvodit rozdíly v chování obou variant systému s dostatečnou věrohodností a experimentální prověřování rozdílů v chování optimalizovaného volebního systému oproti stávajícímu volebnímu systému již nepřinese další výsledky významné pro rozhodnutí o výhodnoti provedení uvedené optimalizace.

## 6. Shrnutí simulačních experimentů a závěr ##

Z výsledků experimentů jednoznačně vyplývá že snížení času potřebného k sečtení a distribuci výsledků hlasování přímým napojením okrskových počítačů do počítačové sítě se serverem ČSÚ by (vyvážilo/nevyvážilo) náklady nutné k technické realizaci takové optimalizace.

