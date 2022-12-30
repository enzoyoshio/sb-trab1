#include "need.h"

namespace helper {

    //pega os tokens
    std::vector<std::string> get_tokens(std::ifstream &file) {
        std::string line;
        getline(file, line);

        std::vector<std::string> tokens = parser(line, ' ');

        return tokens;
    }

    //ARRUMAR
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

    //remove os comentarios
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
        // separa por espaco 
        tokens = parser(s, ' ');

        // se so hover espacos na linha, retorna vazio
        if(tokens.empty()) return {};

        // se o primeiro caractere for diferente de espaco
        // se ';' entao essa eh uma linha de comentario
        if(tokens[0][0] == ';') return {};

        // irei separar pelo token de comentario
        tokens = parser(s, ';');
        // se nao tiver nenhum token
        // retorna vazio
        if(tokens.empty()) return {};

        // pega o primeiro token
        // parseia por espaco
        tokens = parser(tokens[0], ' ');
        return tokens; 
    }

    std::string join(std::vector<std::string> v, char c) {
        std::string ret = "";
        for(auto it: v) ret += it, ret += c;
        if(ret.size()) ret.pop_back();
        return ret;
    }

    //tolower para strings
    std::string tolower(std::string s) {
        std::string ret = "";
        for(auto ch: s) ret += std::tolower(ch);
        return ret;
    }

    //tolower para vetor de string
    std::vector<std::string> tolower(std::vector<std::string> s) {
        std::vector<std::string> ret;
        for(auto word: s) ret.push_back(helper::tolower(word));
        return ret;
    }

    //toupper para strings
    std::string toupper(std::string s) {
        std::string ret = "";
        for(auto ch: s) ret += std::toupper(ch);
        return ret;
    }

    //toupper para vetor de string
    std::vector<std::string> toupper(std::vector<std::string> s) {
        std::vector<std::string> ret;
        for(auto word: s) ret.push_back(helper::toupper(word));
        return ret;
    }

    //converte hexadecimal para inteiro
    int str2num(std::string s) {
        if(s.substr(0, 2) == "0x") {
            std::stringstream ss(s);
            int var;
            ss >> std::hex >> var;
            return var;
        }else return stoi(s);
    }

    //pega proxima linha valida
    std::vector<std::string> get_next_valid_line(std::ifstream &file) {
        std::string s;
        std::vector<std::string> tokens;
        while(getline(file, s)) {
            tokens = helper::remove_comments(s);
            if(!(tokens.empty() || tokens[0][0] == ';')) break;
        }
        return tokens;
    }

    //atualiza ARRUMAR
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

    //remove espacos
    std::string ltrim(std::string s) {
        std::reverse(s.begin(), s.end());
        while(s.size() && s.back() == ' ') s.pop_back();
        std::reverse(s.begin(), s.end());
        return s;
    }
    std::string rtrim(std::string s) {
        while(s.size() && s.back() == ' ') s.pop_back();
        return s;
    }

    std::string trim(std::string s) {
        return ltrim(rtrim(s));
    }

    // de acordo com a especificacao 
    // nao vai ter erro na macro
    std::vector<std::string> get_macro_line(std::string s) {
        std::vector<std::string> ret;
        
        // pega primeiro o label
        std::vector<std::string> token = parser(s, ':');
        ret.push_back(token[0]);

        // separa a macro e args
        token = parser(token[1], ' ');
        ret.push_back(token[0]);

        std::string args = "";
        for(int i = 1; i < token.size(); i++) args += token[i] + " ";
        token = parser(args, ',');
        for(auto& it: token)
            ret.push_back(trim(it));
        return ret;
    }



    int compute(std::string s) {
        std::cout << "WIP" << std::endl;
        return -1;
    }

    bool checkSymbol(std::string s, std::map<std::string, int> ts) {
        // se a string for um simbolo
        if(ts.find(s) != ts.end()) return true;
        // se for um literal
        if(isnumber(s)) return true;

        // se for um vetor estilo X+2
        std::vector<std::string> v = parser(s, '+');
        if(v.size() != 2) return false;

        if(ts.find(v[0]) != ts.end() && isdigit(v[1])) return true;

        return false;
    }

    bool checkSim(std::string s){
        for(char c: s){
            bool flag = true;
            //se for alguma letra do alfabeto, numero ou "_" a letra eh valida para o roluto, se nao ela n eh valida e retorna true
            if( (c >= 'a' and c <='z')
            or (c >= 'A' and c <='Z') 
            or (c >= '0' and c <='9') 
            or (c == '_'))flag = false;
            if(flag) return true;
        }

        return false;
    }

    //isnumber para string
    bool isnumber(std::string s) {
        if(s == "") return false;
        if(s[0] == '+' && isdigit(s.substr(1))) return true;
        if(s[0] == '-' && isdigit(s.substr(1))) return true;
        if(s.substr(0, 2) == "0x" && isdigit(s.substr(2))) return true;
        return isdigit(s);
    }

    //isdigit para string //  QUAL a  diferenca do  is number pra is digit nesse caso? ARRUMAR
    bool isdigit(std::string s) {
        for(auto ch: s)
            if(!std::isdigit(ch))
                return false;
        return true;
    }
}
