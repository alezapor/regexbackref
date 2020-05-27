# regex_matcher

Implementace prostredku pro zpracovani regularnich vyrazu se zpetnymi referencemi.

## Building

```bash
make compile
```

## Spusteni

Pro spusteni zadejte ./regexmatcher <volba algoritmu> <regex> <vstupni retezec>  
Volba algoritmu:  
0 = simpleTM  
1 = simpleMemory  
2 = avdMemory  

###Priklad
```bash
./regex_matcher 1 "X{a*+b*}X*" "aabbbb" 
```

## Testy

Pro spusteni testu zadejte: ./test.sh <nazev_sady>  
Skripty urcene pro spusteni testy v grepu (testGrep.sh) a perlu (testPerl.sh)  
Skript pro mereni casu vypoctu (testTime.sh)  
Pozor! Pokud chcete zmerit cas implementace algoritmu simpleTM, odkomentujte odpovidajici radky ve skriptu testTime.sh (pro sady avd, hard a nVar muze trvat radove desitky minut).  
###Priklady
```bash
./test.sh simpleReg
./testPerl.sh nSigma
./testTime hard
```
Pro spusteni vsech testu zadejte:
```bash
make tests
make time
```
## Generovani dokumentace
Dokumentace ve formatu HTML se vytvori v adresari doc.
```bash
make doc
```
## Uklid
Smazat spustitelne, objektove soubory, soubory dokumentace a vystupy z testu lze prikazem
```bash
make clean
```
