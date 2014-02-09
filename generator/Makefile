# Překladač C
UNAME := $(shell uname -n)
ifeq ($(UNAME), merlin.fit.vutbr.cz)
  CPP = g++-4.8
else
  CPP = g++
endif
CXX=$(CPP)

vystup.csv: generator vstup.csv
	./generator

generator: generator.cpp
	$(CXX) -o $@ $^ -std=c++11 -lsimlib -lm

clean:
	rm -f generator vystup.csv

# EOF
