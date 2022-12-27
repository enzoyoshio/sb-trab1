montador: montador.cpp ./bib/pre_processador.h ./bib/pre_processador.cpp ./bib/processa_macro.h ./bib/processa_macro.cpp ./bib/processa_objeto.h ./bib/processa_objeto.cpp ./bib/helper.h ./bib/helper.cpp
	g++ bib/pre_processador.cpp -c -std=c++17
	g++ bib/processa_objeto.cpp -c -std=c++17
	g++ bib/processa_macro.cpp -c -std=c++17
	g++ bib/helper.cpp -c -std=c++17
	g++ montador.cpp -o montador pre_processador.o processa_objeto.o processa_macro.o helper.o
	rm *.o
