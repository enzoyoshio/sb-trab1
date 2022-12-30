// fazer o processamento para objeto aqui
#pragma once
#include <string>
#include <iostream>
#include <fstream>

namespace processa_objeto {

    struct Erro{
        // Struct Erro 
        // tipo -> tipo de erro
        // mensagem -> mensagem do erro 
        std::string tipo;
        std::string mensagem;

        //ajuda na hora de usar funcoes como o emplace_back, sem a necessidade de criar uma variavel auxiliar
        Erro(std::string tipo, std::string mensagem):tipo(tipo),  mensagem(mensagem){};

    };

    struct Line {
        std::string rotulo;
        std::string operacao;
        std::vector<std::string> operadores;
        bool vazio = 0;
        bool erro = 0;

        bool empty();
        void flush(std::ofstream &fileoutput);
        bool read(std::ifstream &fileinput);
        void update_arg(std::map<std::string, int> indexOf);
        void print();
        Line();
        Line(std::string s);
    };

    struct Montador {

        int contador_linha;
        int contador_posicao_memoria;
        // dada uma instr, informa o opcode dela
        // lembrar de sempre dar tolower
        std::map<std::string, int> tabela_instrucao = {
            {"add", 1},
            {"sub", 2},
            {"mul", 3},
            {"div", 4},
            {"jmp", 5},
            {"jmpn", 6},
            {"jmpp", 7},
            {"jmpz", 8},
            {"copy", 9},
            {"load", 10},
            {"store", 11},
            {"input", 12},
            {"output", 13},
            {"stop", 14}
        };

        // tamanho que a instrucao ocupa na memoria 
        std::map<std::string, int> tabela_tamanho = {
            {"add", 2},
            {"sub", 2},
            {"mul", 2},
            {"div", 2},
            {"jmp", 2},
            {"jmpn", 2},
            {"jmpp", 2},
            {"jmpz", 2},
            {"copy", 3},
            {"load", 2},
            {"store", 2},
            {"input", 2},
            {"output", 2},
            {"stop", 1}
        };

        std::map<std::string, int> tabela_simbolo;

        std::set<std::string> diretivas = {
            "space",
            "const", 
            "section"
        };
        std::vector<short> memory;

        std::vector<Erro> errors;   
        Montador();

        void print_error();
    };

    void print();
    
    void processa(std::string filename);
    Montador passagem1(std::string filename);
    void passagem2(std::string filename, Montador m);

}
