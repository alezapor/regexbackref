# Stručný popis obsahu CD

| adresář/soubor | popis obsahu |
| ------ | ------ |
| BP_Zaporozhchenko_Oleksandr_2020.pdf | text práce |
| src/impl | adresář obsahující zdrojové kódy implementace prostředku pro zpracování regexů|
| src/test.sh | skript určený pro testování správnosti implementací | 
| src/testGrep.sh | skript určený pro spuštění testů v programu grep | 
| src/testPerl.sh | skript určený pro spuštění testů v jazyce Perl | 
| src/testTime.sh | skript určený pro měření času běhu | 
| src/tests | adresář obsahující datové sady použité pro testování |
| src/tests/out | adresář, do kterého se ukládají výstupy z testů |
| src/thesis | adresář obsahující zdrojovou formu práce ve formátu LaTeX |


# regex_matcher -- uživatelská příručka

Implementace prostředku pro zpracování regulárních výrazů se zpetnými referencemi.

## Building
```bash
cd src
```

```bash
make compile
```

## Spuštění

Pro spuštění zadejte `./regexmatcher <volba algoritmu> <regex> <vstupni retezec> `  
**Volba algoritmu:**  
0 = simpleTM  
1 = simpleMemory  
2 = avdMemory  

### Příklad
```bash
./regex_matcher 1 "X{a*+b*}X*" "aabbbb" 
```

## Testy

Pro spuštění testů zadejte: `./test.sh <nazev_sady>`  
Skripty určené pro spuštění testů v grepu (*testGrep.sh*) a perlu (*testPerl.sh*)  
Skript pro měření času výpočtu: *testTime.sh*  
{- Pozor! -} Pokud chcete změřit čas implementace algoritmu simpleTM, odkomentujte odpovídající řádky ve skriptu *testTime.sh* (pro sady **avd**, **hard** a **nVar** může trvat řádově desítky minut).  

### Příklady
```bash
./test.sh simpleReg
./testPerl.sh nSigma
./testTime hard
```
Pro spuštění všech testů zadejte:
```bash
make tests
make time
```
## Generování dokumentace
Dokumentace ve formatu HTML se vytvoří v adresari *doc*.
```bash
make doc
```
## Úklid
Smazat spustitelné, objektové soubory, soubory dokumentace a výstupy z testů lze příkazem
```bash
make clean
```
