#include "need.h"

namespace pre_processador {
    void print() {
        std::cout << "fazendo o pre processamento :)" << std::endl;
    }

    // tem que passar o filename sem extensao
    void processa(std::string filename) {
        
        //adiciona a nova extensao
        std::string finput = filename + ".ASM";
        std::ifstream fileinput(finput);

        // Se o arquivo nao existir ou n estiver aberto
        if(!fileinput.is_open()) {
            std::cout << "arquivo nao existe :(" << std::endl;
            return;
        }

        std::string foutput = filename + ".PRE";
        std::ofstream fileoutput(foutput);

        // de acordo com a especificacao
        // o EQU vai ser estar fora da secao TEXT
        //if e equ sempre serao usados de forma correta
        // no inicio do arquivo
        // basta acreditar que os testes serao assim
        std::map<std::string, int> tabela; 
        processa_objeto::Line linha;
        while(linha.read(fileinput)) {
            // ignora linha em branco
            if(linha.empty()) continue;
            // vai dar erro, se der tempo corrigir, ARRUMAR, que tipo de  erro daria?
            if(linha.operacao == "equ") {//se for equ, transforma em numero, pois le como string
                tabela[linha.rotulo] = helper::str2num(linha.operadores[0]);
            }else if(linha.operacao == "if") {//ARRUMAR
                processa_objeto::Line linha2(helper::join(helper::get_next_valid_line(fileinput), ' '));
                if(tabela[linha.operadores[0]])
                    linha2.flush(fileoutput);
            }else {
                for(auto &it: linha.operadores) // para cada operador
                    if(tabela.find(it) != tabela.end())
                        it = std::to_string(tabela[it]);
                linha.flush(fileoutput);
            }
        }

        fileinput.close();
        fileoutput.close();
    }
}
