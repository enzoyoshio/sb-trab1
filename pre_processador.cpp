#include "pre_processador.h"
#include <string>
#include <iostream>
#include <fstream>
#include "helper.h"
#include <map>

namespace pre_processador {
    void print() {
        std::cout << "fazendo o pre processamento :)" << std::endl;
    }

    // tem que passar o filename sem extensao
    void processa(std::string filename) {
        
        std::string finput = filename + ".ASM";
        std::ifstream fileinput(finput);

        if(!fileinput.is_open()) {
            std::cout << "arquivo nao existe :(" << std::endl;
            return;
        }

        std::string foutput = filename + ".PRE";
        std::ofstream fileoutput(foutput);

        // de acordo com a especificacao
        // o EQU vai ser estar fora da secao TEXT
        // no inicio do arquivo
        // basta a gente acreditar que os testes serao assim
        std::string line;
        std::map<std::string, int> tabela; 

        // talvez pode trocar essas duas proximas linhas por
        // while(std::vector<std::string> tokens = helper::remove_comments(helper::get_next_valid_line(fileinput)))
        // mas tem que testar pra ver se da certo
        // mas acho q sim
        while(getline(fileinput, line)) {
            std::vector<std::string> tokens = helper::remove_comments(line);
            
            // ignora linha em branco
            if(tokens.empty()) continue;
            
            tokens = helper::tolower(tokens);

            // tem um equ
            // vou fazer considerando que ele so vai colocar valores numericos
            // se sobrar tempo eu implemento pra fazer uma expressao 
            if(tokens.size() > 1 && tokens[1] == "equ") {

                // removendo os ':' que indicam que eh uma label
                // std::string tkn = tokens[0].substr(0, tokens[0].size()-1);
                std::string tkn = tokens[0];
                tkn.pop_back();    

                // aqui ja verifica se tem token repetido?
                // ou sera q nunca vai ter?
                tabela[tkn] = helper::str2num(tokens[2]);        
            }else if(tokens[0] == "if") {
                // caso seja uma expressao valida
                // mantenho a proxima linha
                if(tabela[tokens[1]]) {
                    tokens = helper::get_next_valid_line(fileinput);
                    tokens = helper::tolower(tokens);
                    helper::flushline<std::string>(fileoutput, tokens);
                }else {
                    // caso nao seja uma expressao valida, apagar a proxima linha valida
                    tokens = helper::get_next_valid_line(fileinput);
                }
            }else {
                // caso nao seja uma linha de equ nem de if
                // retirar os comentarios e seguir a vida
                helper::flushline<std::string>(fileoutput, tokens);
            }
        }
    }
}
