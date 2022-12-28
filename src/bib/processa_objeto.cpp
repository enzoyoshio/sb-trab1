#include "need.h"

namespace processa_objeto {
    void print() {
        std::cout << "transformando o codigo fonte em objeto :)" << std::endl;
    }

    void processa(std::string filename) {
        
        // passagem 1 retorna um montador com a tabela de simbolos montada;
        Montador m = passagem1(filename);
        // passagem 2 produz o file.OBJ dado o filename e a tabela de simbolos
        // montada
        passagem2(filename, m);
    }
    
    Montador::Montador() {
        this->memory.assign(216, -1);
        this->contador_linha = 1;
        this->contador_posicao_memoria = 0;
        this->linha = Line();
    }

    Line::Line() {}

    Line::Line(std::string s) {

        auto pos = s.find(":");
        
        // caso nao exista label
        if(pos == std::string::npos) {
            this->rotulo = ""; // nao existe rotulo
            std::vector<std::string> tok = helper::parser(s, ' ');
            this->operacao = tok[0];
            if(this->operacao == "copy") {
                tok = helper::parser(tok[1], ',');
                for(auto it: tok)
                    this->operadores.push_back(it);
            }else 
                for(int i = 1; i < tok.size(); i++)
                    this->operadores.push_back(tok[i]);
                
        }else {
            std::vector<std::string> tok = helper::parser(s, ':');

            if(tok.size() > 2) {
                std::cout << "erro, mais de uma label criada na mesma linha" << std::endl;
            }
            // significa que so tem o rotulo nessa linha
            this->rotulo = tok[0];
            // considerar q nunca tem token sozinho, 
            // depois eu vejo como arrumo isso 

            tok = helper::parser(tok[1], ' ');
            this->operacao = tok[0]; 
            // adicionando o resto dos argumento;

            if(this->operacao == "copy") {
                tok = helper::parser(tok[2], ',');
                for(auto it: tok)
                    this->operadores.push_back(it);
            }else
                for(int i = 1; i < tok.size(); i++) 
                    this->operadores.push_back(tok[i]);
           
        }
    
    }

    Montador passagem1(std::string filename) {
        
        Montador m{};

        std::string finput = filename + ".MCR";
        std::ifstream fileinput(finput);

        if(!fileinput) {
            std::cout << "arquivo nao encontrado :(" << std::endl;
            return Montador{};
        }

        while(fileinput) {
            std::string linha;
            getline(fileinput, linha);
            std::cout << "inicio do while "<< linha << std::endl;
            if(linha == "") break; // supposebly its the last line
            m.linha = Line(linha);

            std::cout << "\n\nflushing line " << linha << std::endl;
            std::cout << "line count = " << m.contador_linha << std::endl;
            std::cout << "rotulo = " << m.linha.rotulo << std::endl;
            std::cout << "operacao = " << m.linha.operacao << std::endl;
            for(auto proc: m.linha.operadores)
                std::cout << "operador = " << proc << std::endl;
                
            // isso aqui ta confuso 
            // mas é pra resolver o esquema de ter apenas o rotulo em uma linha
            // while(m.linha.operacao == "neednextline") {
            //     getline(fileinput, linha);
            //     Line newLine(linha);
            //     if(newLine.rotulo != "") {
            //         // acionar erro aqui, dois rotulos seguidos
            //     }
            // }

            if(m.linha.operacao == "section") {
                m.contador_linha++;
                continue;
            }
            // se existe rotulo
            if(m.linha.rotulo != "") {
                if(m.tabela_simbolo.find(m.linha.rotulo) != m.tabela_simbolo.end()) {
                    // jogar erro, redefinicao de simbolo
                    std::cout << "erro, redefinicao do simbolo " << m.linha.rotulo 
                    << " na linha " << m.contador_linha << std::endl;
                }else {
                    // se nao, eu digo onde na memoria esse rotulo esta especificando
                    m.tabela_simbolo[m.linha.rotulo] = m.contador_posicao_memoria;
                }

                if(m.linha.rotulo == "old_data") {
                    std::cout << "POrra ta aqui " << m.tabela_simbolo[m.linha.rotulo] << std::endl;
                }
            }

            // caso a operacao esteja na tabela de instrucao
            if(m.tabela_instrucao.find(m.linha.operacao) != m.tabela_instrucao.end()) {
                std::cout << "oi?\n";
                std::cout << m.linha.operacao << std::endl;
                std::cout << m.tabela_tamanho[m.linha.operacao] << std::endl;
                m.contador_posicao_memoria += m.tabela_tamanho[m.linha.operacao];
                std::cout << "cheguei do outro lado\n";
            }else {
                // fazer para diretivas 
                // const e space 
                // preguica depois eu faco
                if(m.diretivas.find(m.linha.operacao) != m.diretivas.end()) {
                    if(m.linha.operacao == "space" && m.linha.operadores.size() == 1) 
                        m.contador_posicao_memoria += helper::str2num(m.linha.operadores[0]);
                    else
                        m.contador_posicao_memoria++;
                }else {
                    // se nao achar nem na diretiva
                    // jogar erro
                    // operacao nao identificada
                    std::cout << "operacao " << m.linha.operacao << " nao identificada na linha " << m.contador_linha << std::endl;
                }
            }
            std::cout << "fim do while\n";
            m.contador_linha += 1;
        }

        return m;
    }
    
    void passagem2(std::string filename, Montador m) {

        std::cout << "cheguei na passagem2\n";

        m.contador_posicao_memoria = 0;
        m.contador_linha = 1;

        std::string finput = filename + ".MCR";
        std::ifstream fileinput(finput);

        if(!fileinput) {
            std::cout << "arquivo nao encontrado :(" << std::endl;
            return;
        }

        while(fileinput) {
            std::string linha;
            getline(fileinput, linha);
            if(linha == "") break;
            m.linha = Line(linha);

            if(m.linha.operacao == "section") {
                m.contador_linha++;
                continue;
            }

            // verificar se tem um simbolo fora da tabela de simbolo
            // jogar erro se tiver
            for(auto sim: m.linha.operadores) {
                if(m.tabela_simbolo.find(sim) == m.tabela_simbolo.end()) {
                    try {
                        int a = helper::str2num(sim);
                    } catch (...) {
                        std::cout << "erro, simbolo " << sim << " nao definido" << std::endl;          
                    }
                }
            }

            if(m.tabela_instrucao.find(m.linha.operacao) != m.tabela_instrucao.end()) {

                // checar se tem a quantidade certa de operandos
                if(m.linha.operadores.size() != m.tabela_tamanho[m.linha.operacao]-1) {
                    // jogar erro aqui
                    // como vou verificar se os tipos estao certos?
                    std::cout << "quantidade de argumentos invalida na linha " << m.contador_linha << std::endl;
                }

                // preenchendo na memoria
                m.memory[m.contador_posicao_memoria++] = m.tabela_instrucao[m.linha.operacao];
                for(auto tkn: m.linha.operadores) {
                    if(m.tabela_simbolo.find(tkn) != m.tabela_simbolo.end()) {
                        m.memory[m.contador_posicao_memoria++] = m.tabela_simbolo[tkn];
                    }else {
                        m.memory[m.contador_posicao_memoria++] = helper::str2num(tkn);
                    }
                }
            }else {
                if(m.diretivas.find(m.linha.operacao) != m.diretivas.end()) {
                    if(m.linha.operacao == "space") {
                        if(m.linha.operadores.empty()) 
                            m.memory[m.contador_posicao_memoria++] = 0;
                        else {
                            try {
                                for(int i = 0; i < helper::str2num(m.linha.operadores[1]); i++)
                                    m.memory[m.contador_posicao_memoria++] = 0;
                            } catch (...) {
                                std::cout << "deu errado aqui no space" << std::endl;
                            }
                        }
                    }else {
                        try {
                            m.memory[m.contador_posicao_memoria++] = helper::str2num(m.linha.operadores[0]);
                        } catch (...) {
                            std::cout << "simbolo invalido para constante" << std::endl;
                        }
                    }
                }else {
                    std::cout << "operacao nao identificada" << std::endl;
                }
            }
            m.contador_linha++;
        }

        std::ofstream fileoutput(filename + ".OBJ");
        while(m.memory.back() == -1) m.memory.pop_back(); // gambiarra pra dar certo
        for(int i = 0; i < m.memory.size(); i++) if(m.memory[i] == -1) m.memory[i] = 0; // mais gambiarra
        helper::flushline<short>(fileoutput, m.memory);
    }

}
