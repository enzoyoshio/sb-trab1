#include "helper.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

std::vector<std::string> helper::get_tokens(std::ifstream &file) {
    std::string line;
    getline(file, line);

    std::vector<std::string> tokens = parser(line, ' ');

    int idx = tokens.size();

    for(int i = (int)tokens.size()-1; i >= 0; i--) {
        if(tokens[i] == ";") {
            idx = i;
            break;
        }
    }

    while(tokens.size() > idx) tokens.pop_back();
    return tokens; 
}

std::vector<std::string> helper::parser(std::string s, char c) {
    std::stringstream check(s);
    std::string aux;
    std::vector<std::string> ret;

    while(getline(check, aux, c)) {
        if(aux != "") 
            ret.push_back(aux);
    }

    return ret;
}
