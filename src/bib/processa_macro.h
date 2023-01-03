// processa macro aqui
#pragma once
#include <string> 
#include <iostream>
#include <vector>
#include <map>
#include "processa_objeto.h"

namespace processa_macro {

    // imprime informando que estamos
    // fazendo o pre processamento de macro
    void print();

    // faz o processamento de macros
    void processa(std::string filename);

    // substitui o chamado da macro pela sua implementacao
    void flush_macro(std::ifstream &inputfile, 
                     std::ofstream &outputfile, 
                     std::map<std::string, int> MNT,
        std::map<int, std::vector<processa_objeto::Line>> MDT,
                     processa_objeto::Line linha);

    // salva a implementacao da macro na MDT
    void save_macro(std::ifstream &fileinput, 
                     std::map<std::string, int> &MNT,
        std::map<int, std::vector<processa_objeto::Line>> &MDT, 
                     int &counter,
                     processa_objeto::Line linha);
}
