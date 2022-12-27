#include "processa_objeto.h"
#include <string>
#include <iostream>

namespace processa_objeto {
    void print() {
        std::cout << "transformando o codigo fonte em objeto :)" << std::endl;
    }

    void processa(std::string filename) {
        auto coisas = passagem1(filename);
        passagem2(filename, coisas);
    }
}
