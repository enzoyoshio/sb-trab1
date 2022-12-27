#include "need.h"

namespace processa_macro {
    void print() {
        std::cout << "processando macro :)" << std::endl;
    }

    void processa(std::string filename) {
        
        std::string finput = filename + ".PRE";
        std::ifstream fileinput(finput);

        if(!fileinput.is_open()) {
            std::cout << "Arquivo inexistente :(" << std::endl;
            return;
        }

        std::string foutput = filename + ".MCR";
        std::ofstream fileoutput(foutput);

        std::map<std::string, int> MNT;
        std::map<int, std::vector<std::vector<std::string>>> MDT;

        int counter = 0;
        std::string line;
        while(getline(fileinput, line)) {
            std::vector<std::string> tokens = helper::remove_comments(line);

            if(tokens.empty()) continue;

            tokens = helper::tolower(tokens);

            // definicao de macro
            if(tokens.size() > 1 && tokens[1] == "macro") {

                tokens[0].pop_back();
                MNT[tokens[0]] = ++counter;

                int indexCounter = 1;
                std::map<std::string, int> indexOf;
                for(int i = 2; i < tokens.size(); i++)
                    indexOf[tokens[i]] = indexCounter++;

                while(tokens[0] != "endmacro") {
                    tokens = helper::get_next_valid_line(fileinput);
                    tokens = helper::tolower(tokens);
                    tokens = helper::update_arg(tokens, indexOf);
                    if(tokens[0] != "endmacro")
                        MDT[counter].push_back(tokens);
                }

            } // chamado de macro
            else if(MNT.find(tokens[0]) != MNT.end()) {
 
                int indexCounter = 1;
                std::map<std::string, std::string> valueOf;
                for(int i = 1; i < tokens.size(); i++)
                    valueOf["#arg" + std::to_string(i)] = tokens[i];

                for(auto& tokens: MDT[MNT[tokens[0]]]) {
                    for(auto& tkn: tokens)
                        if(valueOf.find(tkn) != valueOf.end())
                            tkn = valueOf[tkn];
                    helper::flushline(fileoutput, tokens);
                }
               
            } // comando nao envolvendo macro
            else {
                helper::flushline(fileoutput, tokens);
            }
        }
        
    }
}
