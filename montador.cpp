#include <iostream>
#include "pre_processador.h"
#include "processa_macro.h"
#include "processa_objeto.h"
#include "helper.h"
#include <fstream>

int main(int argc, char *argv[]) {

  // se nao passar nenhum argumento para o exec
    if(argc == 1) {
        std::cout << "informe uma opcao e um arquivo para ser montado" << std::endl;
        return 0;
    }

    if(argc == 2) {
        std::cout << "informe o arquivo a ser montado, sem extensao" << std::endl;
        return 0;
    }
    

    std::string stringfile(argv[2]);
    stringfile += ".asm";
    std::ifstream file(stringfile);
 
    auto tokens = helper::get_tokens(file);
    auto tokens2 = helper::get_tokens(file);

    for(auto it: tokens) std::cout << it << ' '; std::cout << std::endl;
    for(auto it: tokens2) std::cout << it << ' '; std::cout << std::endl;
    return 0;

    switch(argv[1][1]) {
        case 'p':
        pre_processador::print();
        std::cout << "pre processamento do arquivo " << argv[2] << std::endl;
        break;
        case 'm':
        processa_macro::print();
        std::cout << "processamento de macros do arquivo " << argv[2] << std::endl;
        break;
        case 'o':
        processa_objeto::print();
        std::cout << "criando arquivo objeto do arquivo " << argv[2] << std::endl;
        break;
        // se der tempo fazer um help
        // bem simples por que so tem tres opcoes
        case 'h':
        std::cout << "help" << std::endl;
        break;
        default:
        std::cout << "opcao nao existente, para saber quais opcoes estao disponiveis use './montador -h'" << std::endl;

    }
}
