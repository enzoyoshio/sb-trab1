#include "pre_processador.h"
#include <string>
#include <iostream>
#include <fstream>

void pre_processador::print() {
    std::cout << "fazendo o pre processamento :)" << std::endl;
}

// tem que passar o filename sem extensao
void pre_processador::processa(std::string filename) {
    
    std::finput = filename + ".ASM";
    std::ifstream fileinput(filename);

    if(ifstream.is_open()) {
        std::cout << "arquivo nao existe :(" << std::endl;
        return;
    }

    std::foutput = filename + ".PRE";
    std::ofstream fileoutput(filename)

}
