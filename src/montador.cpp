#include "bib/need.h"

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

    std::string filename{argv[2]};
    switch(argv[1][1]) {
        case 'p':
        pre_processador::print();
        pre_processador::processa(filename);
        break;
        case 'm':
        processa_macro::print();
        processa_macro::processa(filename);
        break;
        case 'o':
        processa_objeto::print();
        processa_objeto::processa(filename);
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
