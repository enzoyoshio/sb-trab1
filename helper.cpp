#include "helper.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>

namespace helper {
    std::vector<std::string> get_tokens(std::ifstream &file) {
        std::string line;
        getline(file, line);

        std::vector<std::string> tokens = parser(line, ' ');

        return tokens;
    }

    std::vector<std::string> parser(std::string s, char c) {
        std::stringstream check(s);
        std::string aux;
        std::vector<std::string> ret;

        while(getline(check, aux, c)) {
            if(aux != "") 
                ret.push_back(aux);
        }

        return ret;
    }

    std::vector<std::string> remove_comments(std::vector<std::string> tokens) {
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

    std::vector<std::string> remove_comments(std::string s) {
        std::vector<std::string> tokens;
        tokens = parser(s, ' ');
        if(tokens.empty()) return {};

        if(tokens[0][0] == ';') return {};

        tokens = parser(s, ';');
        if(tokens.empty()) return {};

        tokens = parser(tokens[0], ' ');
        return tokens; 
    }

    std::string tolower(std::string s) {
        std::string ret = "";
        for(auto ch: s) ret += std::tolower(ch);
        return ret;
    }

    std::vector<std::string> tolower(std::vector<std::string> s) {
        std::vector<std::string> ret;
        for(auto word: s) ret.push_back(helper::tolower(word));
        return ret;
    }

    std::string toupper(std::string s) {
        std::string ret = "";
        for(auto ch: s) ret += std::toupper(ch);
        return ret;
    }

    std::vector<std::string> toupper(std::vector<std::string> s) {
        std::vector<std::string> ret;
        for(auto word: s) ret.push_back(helper::toupper(word));
        return ret;
    }

    /*
    template <typename T>
    std::ofstream &operator<<(std::ofstream &file, std::vector<T> v) {
        for(auto it: v) file << it << ' ';
        file << '\n';

        return file;
    }
    */

    /*
    template <typename T>
    void flushline(std::ofstream &file, std::vector<T> v) {
        for(auto it: v) file << it << ' ';
        file << '\n';
    }
    */

    int str2num(std::string s) {
        if(s.substr(0, 2) == "0x") {
            // hexadecimal
            return -1;
        }else return stoi(s);
    }

    std::vector<std::string> get_next_valid_line(std::ifstream &file) {
        std::string s;
        std::vector<std::string> tokens;
        while(getline(file, s)) {
            tokens = helper::remove_comments(s);
            if(!(tokens.empty() || tokens[0][0] == ';')) break;
        }
        return tokens;
    }

    std::vector<std::string> update_arg(std::vector<std::string> v, std::map<std::string, int> indexOf) {
        std::vector<std::string> ret;

        for(auto it: v) {
            if(indexOf.find(it) != indexOf.end()) 
                ret.push_back("#arg" + std::to_string(indexOf[it]));
            else 
                ret.push_back(it);
        }

        return ret;
    }








    int compute(std::string s) {
        std::cout << "WIP" << std::endl;
        return -1;
    }
}
