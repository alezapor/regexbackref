A regex matcher that supports backreferences. A backreference makes regular expressions much more powerful formalism for defining **not only** regular languages.
Three algorithms were implemented: the first one uses an LBA approach, the other two are based on memory automata described [here](https://arxiv.org/pdf/1903.05896.pdf) by Marcus L. Schmid.

![workflow](https://github.com/alezapor/regexbackref/actions/workflows/cmake.yml/badge.svg?event=push)

## Prerequisutes
* You must have CMake version 3.10 or later installed. 
* Boost System and Test Libraries are also needed to run tests.

## Build
Go to the directory of the CMake source code tree and create a build directory.
```bash
mkdir build
cd build
cmake ..
```

```bash
cmake --build .
```

## Run

Try to use the newly built `matcher` this way: `matcher <algorithm> <regex> <text> `  
**Selecting the algorithm:**  
0 = simpleTM  
1 = simpleMemory  
2 = avdMemory  

### Example with backreferences
```bash
matcher 1 "X{a*+b*}X*" "aabbbb" 
```

## Tests
To run some final tests use `ctest -C Debug` from the binary directory.

