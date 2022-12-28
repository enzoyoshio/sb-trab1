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
        std::map<int, std::vector<processa_objeto::Line>> MDT;

        int counter = 0;
        int db = 0;
        std::string line;
        processa_objeto::Line linha;
        while(linha.read(fileinput)) {
            // std::cout << "\n\nprocessando essa linha\n";
            // linha.print();

            // std::cout << "\n\n";
            // if(MDT.size() > db) {
            //     db = MDT.size();
            //     std::cout << "ese eh uma ocorrencia da mdt\n";
            //     for(auto [id, l]: MDT) {
            //         std::cout << "ocorrencia " << id << '\n';
            //         for(auto c: l)
            //             c.print();
            //     }
            // }

            if(linha.empty()) continue;

            // definicao de macro 
            if(linha.operacao == "macro")
                save_macro(fileinput, MNT, MDT, counter, linha);
            // chamado de macro
            else if(MNT.find(linha.operacao) != MNT.end())
                flush_macro(fileinput, fileoutput, MNT, MDT, linha);
            // comando nao envolvendo macro
            else
                linha.flush(fileoutput);
        }
        fileinput.close();
        fileoutput.close();
        
    }

    // save macro in MNT and MDT
    void save_macro(std::ifstream &fileinput, 
                     std::map<std::string, int> &MNT,
        std::map<int, std::vector<processa_objeto::Line>> &MDT, 
                     int &counter,
                     processa_objeto::Line linha) {
        MNT[linha.rotulo] = ++counter;

        int indexCounter = 1;
        std::map<std::string, int> indexOf;
        for(auto it: linha.operadores) 
            indexOf[it] = indexCounter++;
        
        // std::cout << "\n\nDentro de save macro\n";
        // std::cout << "listando indexOf\n";
        // for(auto [it, id]: indexOf) 
        //     std::cout << "indexOf[ " << it << " ] = " << id << '\n';

        while(linha.read(fileinput) && linha.operacao != "endmacro") {
            // std::cout << "\nAntes de update\n";
            // linha.print();
            linha.update_arg(indexOf);
            // std::cout << "\nDepois do update\n";
            // linha.print();

            MDT[counter].push_back(linha);
        }
    }

    // changes macro caller for its definition
    void flush_macro(std::ifstream &inputfile, 
                     std::ofstream &outputfile, 
                     std::map<std::string, int> MNT,
        std::map<int, std::vector<processa_objeto::Line>> MDT,
        processa_objeto::Line linha) {
        
        // std::cout << "\ndentro da flush macro\n";
        // linha.print();

        std::map<std::string, std::string> valueOf;
        for(int i = 0; i < linha.operadores.size(); i++) 
            valueOf["#arg" + std::to_string(i+1)] = linha.operadores[i];

        // std::cout << "\n";
        // for(auto [id, val]: valueOf) 
        //     std::cout << "valueOf[ " << id << " ] = " <<  val << '\n';

        // for(int i = 0; i < tokens.size(); i++)
        //     valueOf["#arg" + std::to_string(i)] = tokens[i];

        for(auto &l: MDT[MNT[linha.operacao]]) {

            // std::cout << "\n\nantes de att\n";
            // l.print();
            for(auto& op: l.operadores)
                if(valueOf.find(op) != valueOf.end()) 
                    op = valueOf[op];
            // std::cout << "\napos atualizar\n";
            // l.print();

            if(MNT.find(l.operacao) != MNT.end()) 
                flush_macro(inputfile, outputfile, MNT, MDT, l);
            else l.flush(outputfile);
        }
    }
}
