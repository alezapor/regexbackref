PROGRAM=regexmatcher

CC=g++
CFLAGS=-std=c++14 -Wall -pedantic -Wno-long-long -O0 -ggdb

all: compile doc

run: compile
	./$(PROGRAM)

tests: compile
	./test.sh simple
	./test.sh simpleReg
	./test.sh nVar
	./test.sh avd
	./test.sh nSigma
	./test.sh hard 

time: compile
	./testTime.sh simple
	./testTime.sh simpleReg
	./testTime.sh nVar
	./testTime.sh avd
	./testTime.sh nSigma
	./testTime.sh hard 

compile: $(PROGRAM)

doc: impl/ast.cpp impl/ast.h impl/automaton.cpp impl/automaton.h impl/avdfa.cpp impl/avdfa.h impl/lexer.cpp impl/lexer.h impl/main.cpp impl/matcher.cpp impl/matcher.h impl/ndtm.cpp impl/ndtm.h impl/parser.cpp impl/parser.h impl/tape.cpp impl/tape.h	
	doxygen Doxyfile

$(PROGRAM): objs/main.o objs/matcher.o objs/parser.o objs/ast.o objs/ndtm.o objs/avdfa.o objs/tape.o objs/automaton.o objs/lexer.o objs/memorystate.o
	$(CC) $(CFLAGS) $^ -o $@

objs/memorystate.o: impl/memorystate.cpp impl/memorystate.h |objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/lexer.o: impl/lexer.cpp impl/lexer.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/automaton.o: impl/automaton.cpp impl/automaton.h impl/memorystate.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/tape.o: impl/tape.cpp impl/tape.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/avdfa.o: impl/avdfa.cpp impl/avdfa.h impl/automaton.h impl/tape.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/ndtm.o: impl/ndtm.cpp impl/ndtm.h impl/automaton.h impl/tape.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/ast.o: impl/ast.cpp impl/ast.h impl/ndtm.h impl/avdfa.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/parser.o: impl/parser.cpp impl/parser.h impl/lexer.h impl/ast.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/matcher.o: impl/matcher.cpp impl/matcher.h impl/parser.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/main.o: impl/main.cpp impl/matcher.h impl/parser.h | objs
	$(CC) $(CFLAGS) -c $< -o $@


objs: 
	mkdir objs
	
clean:
	rm -rf $(PROGRAM) objs/ tests/out/ doc/ 2>/dev/null 



