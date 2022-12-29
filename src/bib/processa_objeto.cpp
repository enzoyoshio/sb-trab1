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
    }

    void Line::update_arg(std::map<std::string, int> indexOf) {
        this->operadores = helper::update_arg(this->operadores, indexOf);
    }

    bool Line::read(std::ifstream &fileinput) {
        // if(fileinput.eof()) return false;
        std::string s;
        getline(fileinput, s);
        *this = (Line(s));
        return (!fileinput.eof()) || (!this->empty());
    }

    void Line::flush(std::ofstream &fileoutput) {
        if(this->vazio) return;

        if(this->rotulo != "") 
            fileoutput << this->rotulo << ": ";

        if(this->operacao != "") {
            fileoutput << this->operacao << " ";
            char c = ' ';
            if(this->operacao == "copy" || this->operacao == "macro")
                c = ',';

            std::string s;
            for(auto it: this->operadores)
                s += it, s += c;
            if(s.size()) {
                s.pop_back();
                fileoutput << s;
            }
            fileoutput << '\n';

        }

    }

    bool Line::empty() {
        return this->vazio;
    }

    Line::Line() {}

    // fazer check aqui lexico
    Line::Line(std::string s) {

        s = helper::join(helper::remove_comments(s), ' ');

        if(s.empty()) {
            this->vazio = true;
            return;
        }
        // std::cout << "\n\ndentro do line cosntructor\n";
        // std::cout << s << '\n';

        auto pos = s.find(":");
        std::vector<std::string> tok;
        // caso nao exista label
        if(pos == std::string::npos) {
            this->rotulo = "";
            tok = helper::parser(s, ' ');
        }else {
            tok = helper::parser(s, ':');
            this->rotulo = tok[0];
            if(tok.size() > 2) this->erro = true; // erro duas label na msm linha
            tok = helper::parser(tok[1], ' ');
        }
        
        if(tok.size()) {
            this->operacao = tok[0];
            // vou levar aluns pontos em consideracao
            if(this->operacao == "copy") {
                tok = helper::parser(tok[1], ',');
            }else {
                tok.erase(tok.begin());
                if(this->operacao == "macro") {
                    tok = helper::parser(helper::join(tok, ' '), ',');
                }
            } 
            for(auto it: tok)
                this->operadores.push_back(it);    
        }

        this->rotulo = helper::trim(helper::tolower(this->rotulo));
        this->operacao = helper::trim(helper::tolower(this->operacao));
        for(auto &it: this->operadores)
            it = helper::trim(helper::tolower(it));
        //FAZER ERRO LEXIO
        // this->print();
    }

    void Line::print() {
        std::cout << "rotulo = " << this->rotulo << '\n';
        std::cout << "operacao = " << this->operacao << '\n';
        for(auto it: this->operadores)
            std::cout << "operadores = " << it << ' ';
        std::cout << '\n';
    }

    Montador passagem1(std::string filename) {
        
        Montador m{};

        auto& [linecount, mempos, t_inst, t_size, ts, t_dir, mem, err] = m;

        linecount = 1;
        mempos = 0;

        std::string finput = filename + ".MCR";
        std::ifstream fileinput(finput);

        if(!fileinput) {
            // std::cout << "arquivo nao encontrado :(" << std::endl;
            return Montador{};
        }

        Line linha;
        while(linha.read(fileinput))  {
            auto& [rot, ope, ops, vazio, erro] = linha;

            // se existe algum rotulo
            if(rot != "") {

                // se eu ja tiver definido esse simbolo
                if(ts.find(rot) != ts.end()) {
                    // jogar erro, redefinicao de simbolo
                    // std::cout << "erro, redefinicao do simbolo " << linha.rotulo 
                    // << " na linha " << linecount << std::endl;
                    err.emplace_back("semantico", "redefinicao do simbolo " + rot + 
                            " na linha " + std::to_string(linecount));
                }else { // nao defini ainda, vou definir agora
                    ts[rot] = mempos;
                }
            }

            // verificar a operacao
            if(ope == "") { 
                // caso da pessoa ter dado enter,
                // nao sei anida como resolver
            } // se eu achar essa operacao na tabela de instrucao 
            else if(t_inst.find(ope) != t_inst.end()) {
                mempos += t_size[ope];
            }else {

                // se a operacao for uma diretiva
                if(t_dir.find(ope) != t_dir.end()) {
                    if(ope == "space") {
                        if(ops.size() > 1) {
                            // adicionar erro, quantidade de parametros muito grande
                        }else if(ops.size() == 1) {
                            mempos += helper::str2num(ops[0]);
                        }else linecount++;
                    } 
                    else if(ope == "const")
                        m.contador_posicao_memoria++;
                }else {
                    std::cout << "operacao " << ope 
                    << " nao identificada na linha " << linecount << std::endl;
                    err.emplace_back("sintatico", "operacao " + ope + " nao identificada na linha " + std::to_string(linecount));
                }
            }
            linecount++;
        }
        fileinput.close();

        return m;
    }
    
    void passagem2(std::string filename, Montador m) {
        
        auto& [linecount, mempos, t_inst, t_size, ts, t_dir, mem, err] = m;


        std::cout << "cheguei na passagem2\n";

        mempos = 0;
        linecount = 1;

        std::string finput = filename + ".MCR";
        std::ifstream fileinput(finput);

        if(!fileinput) {
            std::cout << "arquivo nao encontrado :(" << std::endl;
            return;
        }

        Line linha;
        while(linha.read(fileinput)) {

        }

        // depois ver de refatorar isso 
        std::ofstream fileoutput(filename + ".OBJ");
        while(m.memory.back() == -1) m.memory.pop_back(); // gambiarra pra dar certo
        for(int i = 0; i < m.memory.size(); i++) if(m.memory[i] == -1) m.memory[i] = 0; // mais gambiarra
        
        if(m.errors.size())
            m.print_error();
        else
            helper::flushline<short>(fileoutput, m.memory);
    
        fileoutput.close();

        // *************************************************

        while(fileinput) {
            // std::string linha;
            // getline(fileinput, linha);
            // if(linha == "") break;
            // m.linha = Line(linha);

            // if(m.linha.operacao == "section") {
            //     m.contador_linha++;
            //     continue;
            // }

            // // verificar se tem um simbolo fora da tabela de simbolo
            // // jogar erro se tiver
            // for(auto sim: m.linha.operadores) {
            //     if(m.tabela_simbolo.find(sim) == m.tabela_simbolo.end()) {
            //         try {
            //             int a = helper::str2num(sim);
            //         } catch (...) {
            //             std::cout << "erro, simbolo " << sim << " nao definido" << std::endl;          
            //         }
            //     }
            // }

            // if(m.tabela_instrucao.find(m.linha.operacao) != m.tabela_instrucao.end()) {

            //     // checar se tem a quantidade certa de operandos
            //     if(m.linha.operadores.size() != m.tabela_tamanho[m.linha.operacao]-1) {
            //         // jogar erro aqui
            //         // como vou verificar se os tipos estao certos?
            //         std::cout << "quantidade de argumentos invalida na linha " << m.contador_linha << std::endl;
            //     }

            //     // preenchendo na memoria
            //     m.memory[m.contador_posicao_memoria++] = m.tabela_instrucao[m.linha.operacao];
            //     for(auto tkn: m.linha.operadores) {
            //         if(m.tabela_simbolo.find(tkn) != m.tabela_simbolo.end()) {
            //             m.memory[m.contador_posicao_memoria++] = m.tabela_simbolo[tkn];
            //         }else {
            //             m.memory[m.contador_posicao_memoria++] = helper::str2num(tkn);
            //         }
            //     }
            // }else {
            //     if(m.diretivas.find(m.linha.operacao) != m.diretivas.end()) {
            //         if(m.linha.operacao == "space") {
            //             if(m.linha.operadores.empty()) 
            //                 m.memory[m.contador_posicao_memoria++] = 0;
            //             else {
            //                 try {
            //                     for(int i = 0; i < helper::str2num(m.linha.operadores[1]); i++)
            //                         m.memory[m.contador_posicao_memoria++] = 0;
            //                 } catch (...) {
            //                     std::cout << "deu errado aqui no space" << std::endl;
            //                 }
            //             }
            //         }else {
            //             try {
            //                 m.memory[m.contador_posicao_memoria++] = helper::str2num(m.linha.operadores[0]);
            //             } catch (...) {
            //                 std::cout << "simbolo invalido para constante" << std::endl;
            //             }
            //         }
            //     }else {
            //         std::cout << "operacao nao identificada" << std::endl;
            //     }
            // }
            // m.contador_linha++;
        }

    }

    void Montador::print_error() {
        for(auto [type, msg]: this->errors) {
            std::cout << "\n\nerro do tipo " << type << std::endl;
            std::cout << msg << std::endl;
        } 
    }

}
