#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <map>

namespace helper {

    std::vector<std::string> get_tokens(std::ifstream &file);
    std::vector<std::string> parser(std::string s, char c);
    std::vector<std::string> remove_comments(std::vector<std::string> v);
    std::vector<std::string> remove_comments(std::string s); // acho que essa versao eh melhor q a de cima
    std::string tolower(std::string s);
    std::vector<std::string> tolower(std::vector<std::string> v);
    std::string toupper(std::string s);
    std::vector<std::string> toupper(std::vector<std::string> v);
    int str2num(std::string);
    std::vector<std::string> get_next_valid_line(std::ifstream &file);
    std::vector<std::string> update_arg(std::vector<std::string> v, std::map<std::string,int> m);

    // depois ver overloading
    // std::ofstream &operator<<(std::ofstream &file, std::vector<T> v); 
    // por algum motivo nao to conseguindo fazer isso funcionar jogando a 
    // implementacao no .cpp
    // sou burro?
    template <typename T>
    void flushline(std::ofstream &file, std::vector<T> v) {
        for(auto it: v) file << it << ' '; file << '\n';
    }


    // se der tempo, fazer funcao para computar uma expressao
    int compute(std::string);
    
}
