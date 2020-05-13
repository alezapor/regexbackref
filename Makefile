PROGRAM=regexmatcher

CC=g++
CFLAGS=-std=c++14 -Wall -pedantic -Wno-long-long -O0 -ggdb

all: compile doc

run: compile
	./$(PROGRAM)

compile: $(PROGRAM)

doc: src/ast.cpp src/ast.h src/automaton.cpp src/automaton.h src/avdfa.cpp src/avdfa.h src/lexer.cpp src/lexer.h src/main.cpp src/matcher.cpp src/matcher.h src/ndtm.cpp src/ndtm.h src/parser.cpp src/parser.h src/tape.cpp src/tape.h	
	doxygen Doxyfile

$(PROGRAM): objs/main.o objs/matcher.o objs/parser.o objs/ast.o objs/ndtm.o objs/avdfa.o objs/tape.o objs/automaton.o objs/lexer.o objs/memorystate.o
	$(CC) $(CFLAGS) $^ -o $@

objs/memorystate.o: src/memorystate.cpp src/memorystate.h |objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/lexer.o: src/lexer.cpp src/lexer.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/automaton.o: src/automaton.cpp src/automaton.h src/memorystate.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/tape.o: src/tape.cpp src/tape.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/avdfa.o: src/avdfa.cpp src/avdfa.h src/automaton.h src/tape.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/ndtm.o: src/ndtm.cpp src/ndtm.h src/automaton.h src/tape.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/ast.o: src/ast.cpp src/ast.h src/ndtm.h src/avdfa.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/parser.o: src/parser.cpp src/parser.h src/lexer.h src/ast.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/matcher.o: src/matcher.cpp src/matcher.h src/parser.h | objs
	$(CC) $(CFLAGS) -c $< -o $@

objs/main.o: src/main.cpp src/matcher.h src/parser.h | objs
	$(CC) $(CFLAGS) -c $< -o $@


objs: 
	mkdir objs
	
clean:
	rm -rf $(PROGRAM) objs/ tests/out/* doc/ 2>/dev/null 



