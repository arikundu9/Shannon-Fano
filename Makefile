CPP      = g++
CC       = gcc
ifeq ($(OS),Windows_NT)
	F_BIN	= freqg.exe
	CW_BIN	= codewg.exe
	EN_BIN	= encoder.exe
	DE_BIN	= decoder.exe
else
	F_BIN	= freqg
	CW_BIN	= codewg
	EN_BIN	= encoder
	DE_BIN	= decoder
endif

all: freqg codewordg encoder decoder

freqg: freqg.cpp
	$(CPP) -I $(CURDIR) freqg.cpp -o $(F_BIN)
codewordg: codewg.cpp
	$(CPP) -I $(CURDIR) codewg.cpp -o $(CW_BIN)
encoder: encoder.cpp
	$(CPP) -I $(CURDIR) encoder.cpp -o $(EN_BIN)
decoder: decoder.cpp
	$(CPP) -I $(CURDIR) decoder.cpp -o $(DE_BIN)