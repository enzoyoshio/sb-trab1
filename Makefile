montador: montador.cpp pre_processador.h pre_processador.cpp processa_macro.h processa_macro.cpp processa_objeto.h processa_objeto.cpp helper.h helper.cpp
	g++ pre_processador.cpp -c -std=c++17
	g++ processa_objeto.cpp -c -std=c++17
	g++ processa_macro.cpp -c -std=c++17
	g++ helper.cpp -c -std=c++17
	g++ montador.cpp -o montador pre_processador.o processa_objeto.o processa_macro.o helper.o
	rm *.o
