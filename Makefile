montador: montador.cpp pre_processador.h pre_processador.cpp processa_macro.h processa_macro.cpp processa_objeto.h processa_objeto.cpp
	g++ pre_processador.cpp -c
	g++ processa_objeto.cpp -c
	g++ processa_macro.cpp -c
	g++ montador.cpp -o montador pre_processador.o processa_objeto.o processa_macro.o
