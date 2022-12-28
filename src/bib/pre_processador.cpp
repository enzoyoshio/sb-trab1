#include "need.h"

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

        while(getline(fileinput, line)) {
            processa_objeto::Line linha(line);
            
            // ignora linha em branco
            if(linha.empty()) continue;
            
            // de acordo com a especificacao
            // if e equ sempre serao usados de forma correta
            // fazendo como se n tivesse erro desse tipo
            // se tiver uma linha
            // rotulo: equ 
            // vai dar erro, se der tempo corrigir
            if(linha.operacao == "equ") {
                tabela[linha.rotulo] = helper::str2num(linha.operadores[0]);
            }else if(linha.operacao == "if") {
                processa_objeto::Line linha2(helper::join(helper::get_next_valid_line(fileinput), ' '));
                if(tabela[linha.operadores[0]])
                    linha2.flush(fileoutput);
            }else {
                for(auto &it: linha.operadores)
                    if(tabela.find(it) != tabela.end())
                        it = std::to_string(tabela[it]);
                linha.flush(fileoutput);
            }
        }
    }
}
