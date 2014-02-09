# -*- coding: utf-8 -*-
#
# Makefile pro projekt Volby
#

# Jméno přeloženého programu
program=volby

# Úroveň množství debugovacích informací
LDB=-g3 -DDEBUG

# Úroveň optimalizace
# OPTIMIZING=-O2

# Seznam ostatních souborů.
OTHER=Makefile

# Překladač C
UNAME := $(shell uname -n)
ifeq ($(UNAME), merlin.fit.vutbr.cz)
  CPP = g++-4.8
else
  CPP = g++
endif
CXX=$(CPP)

# Link
LINK=-lsimlib -lm

# Makra
# MACROS=-D_XOPEN_SOURCE -D_XOPEN_SOURCE_EXTENDED

# Nepovinné parametry překladače
BRUTAL=-Wall -Wextra -Wno-unused-variable #-Werror
CXXFLAGS=-std=c++11 $(STRICT) -pedantic $(MACROS)

SOURCES=main okrsek sit global

OUTPUT=histogramy

#######################################################################
.PHONY: build debug

# Zkompiluje program (výchozí)
build: generate $(program)

# Zkompiluje program s debugovacími symboly
debug: generate $(program)-dbg

$(program): $(addprefix obj/,$(addsuffix .o,$(SOURCES)))
	$(CXX) -o $@ $^ $(CXXFLAGS) $(OPTIMIZING) $(LINK)

$(program)-dbg: $(addprefix obj/dbg/,$(addsuffix .o,$(SOURCES)))
	$(CXX) -o '$(program)-dbg' $^ $(CXXFLAGS) $(BRUTAL) $(LDB) $(LINK)

obj/%.o: src/%.cpp
	mkdir -p dep obj # Adresare nejsou v gitu
	$(CXX) -MMD -MP -MF dep/$*.d -c -o $@ $< $(CXXFLAGS) $(OPTIMIZING) -DNDEBUG

obj/dbg/%.o: src/%.cpp
	mkdir -p dep/dbg obj/dbg # Adresare nejsou v gitu
	$(CXX) -MMD -MP -MF dep/dbg/$*.d -c -o $@ $< $(CXXFLAGS) $(LDB)

-include $(addprefix dep/,$(addsuffix .d,$(SOURCES)))
#######################################################################
.PHONY: generate histogramy latex strict

generate:
	make -C generator

histogramy: run
	cp dlouho.out '$(OUTPUT)/s PM.out'
	cp kratko.out '$(OUTPUT)/bez PM.out'
	cd histogramy; ./make_more_histograms.sh

latex: histogramy
	make -B -C doc/LaTeX

strict:
	make "STRICT=$(BRUTAL)"

.PHONY: run runMini runNadlouho runNakratko
# Spustí testy
run: runNadlouho runNakratko

runMini: build
	./$(program) generator/vystupmini.csv outputmini.txt

runNadlouho: build
	./$(program) generator/vystup.csv dlouho.out dl

runNakratko: build
	./$(program) generator/vystup.csv kratko.out


.PHONY: valgrind valgrindLeak valgrindReachable gdb kdbg

valgrind: debug
	valgrind --tool=memcheck --leak-check=no --show-reachable=no './$(program)-dbg' generator/vystupmini.csv 'output.txt'

valgrindLeak: debug
	valgrind --tool=memcheck --leak-check=yes --show-reachable=no './$(program)-dbg' generator/vystupmini.csv 'output.txt'

valgrindReachable: debug
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes './$(program)-dbg' generator/vystupmini.csv 'output.txt'

gdb: debug
	gdb './$(program)-dbg' generator/vystup.csv 'output.txt'

kdbg: debug
	kdbg './$(program)-dbg' generator/vystup.csv 'output.txt'


.PHONY: clean pack

clean:
	rm -f obj/*.o obj/dbg/*.o dep/*.d dep/dbg/*.d './$(program)' './$(program)-dbg' 'output.txt'
	rm -f histogramy/*.out histogramy/*.auto.csv histogramy/*.eps
	make -C generator clean
	make -C doc/LaTeX clean
	rm -f pack/$(program) pack/*.*

# Smaže vše co není v GITu - interaktivní režim
clean_all:
	git clean -X -d -i

pack: generate latex
	make -C generator clean
	rm -f pack/$(program) pack/*.*
	mkdir -p pack/src pack/generator
	bash -c 'cp src/*[^~] pack/src/; cp generator/*[^~] pack/generator/; cp doc/LaTeX/imsvolby.pdf pack/dokumentace.pdf;\
	cp doc/LaTeX/imsvolby.tex pack/dokumentace.tex; cp zdroje/database.sql pack/generator/; cd pack; tar -zcvf ../09_xdolez52_xkalin03.tar.gz *'

# EOF
