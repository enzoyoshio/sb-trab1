#include "need.h"

namespace processa_macro {

    void print() {
        std::cout << "processando macro :)" << std::endl;
    }

    void processa(std::string filename) {

        // abre o filename com a extensao .PRE
        std::string finput = filename + ".PRE";
        std::ifstream fileinput(finput);

        // Se o ifstream nao conseguir abrir o arquivo
        if(!fileinput.is_open()) {
            std::cout << "Arquivo inexistente :(" << std::endl;
            return;
        }

        // abrindo/criando o arquivo destino
        std::string foutput = filename + ".MCR";
        std::ofstream fileoutput(foutput);

        // maps para guardar MNT e MDT
        std::map<std::string, int> MNT;
        std::map<int, std::vector<processa_objeto::Line>> MDT;

        // contador para indexar as macros
        int counter = 0;
        // struct Line representando linha
        processa_objeto::Line linha;

        // enquanto houver linhas para ler
        while(linha.read(fileinput)) {

            //se a linha for vazia
            if(linha.empty()) continue;

            // processa se for uma definicao de macro 
            if(linha.operacao == "macro")
                save_macro(fileinput, MNT, MDT, counter, linha);
            // processa se for um chamado de macro
            else if(MNT.find(linha.operacao) != MNT.end())
                flush_macro(fileinput, fileoutput, MNT, MDT, linha);
            // comando nao envolvendo macro
            // apenas imprime a linha
            else
                linha.flush(fileoutput);
        }
        fileinput.close();
        fileoutput.close();
        
    }

    void save_macro(std::ifstream &fileinput, 
                     std::map<std::string, int> &MNT,
        std::map<int, std::vector<processa_objeto::Line>> &MDT, 
                     int &counter,
                     processa_objeto::Line linha) {

        linha.print();

        MNT[linha.rotulo] = ++counter;

        int indexCounter = 1;
        std::map<std::string, int> indexOf;
        for(auto it: linha.operadores) 
            indexOf[it] = indexCounter++;
        

        while(linha.read(fileinput) && linha.operacao != "endmacro") {
            linha.update_arg(indexOf);
            MDT[counter].push_back(linha);
        }
    }

    void flush_macro(std::ifstream &inputfile, 
                     std::ofstream &outputfile, 
                     std::map<std::string, int> MNT,
        std::map<int, std::vector<processa_objeto::Line>> MDT,
        processa_objeto::Line linha) {

        std::map<std::string, std::string> valueOf;
        for(int i = 0; i < linha.operadores.size(); i++) 
            valueOf["#arg" + std::to_string(i+1)] = linha.operadores[i];

        for(auto &l: MDT[MNT[linha.operacao]]) {

            for(auto& op: l.operadores)
                if(valueOf.find(op) != valueOf.end()) 
                    op = valueOf[op];

            if(MNT.find(l.operacao) != MNT.end()) 
                flush_macro(inputfile, outputfile, MNT, MDT, l);
            else l.flush(outputfile);
        }
    }
}
