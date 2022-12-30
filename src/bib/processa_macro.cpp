#include "need.h"

namespace processa_macro {
    //mensagem para alertar que o processamento da macro ocorreu

    //ARRUMAR, precisa desse cout e do "arquivo inexistente :("?
    void print() {
        std::cout << "processando macro :)" << std::endl;
    }

    void processa(std::string filename) {

        //pega o nome do arquivo e adiciona a extensao
        std::string finput = filename + ".PRE";
        std::ifstream fileinput(finput);

        //se nao houver arquivo, ou ele n estiver aberto
        if(!fileinput.is_open()) {
            std::cout << "Arquivo inexistente :(" << std::endl;
            return;
        }
        //pega o nome do arquivo e adiciona a extensao

        std::string foutput = filename + ".MCR";
        std::ofstream fileoutput(foutput);

        std::map<std::string, int> MNT;
        std::map<int, std::vector<processa_objeto::Line>> MDT;

        int counter = 0;
        int db = 0;
        std::string line;
        processa_objeto::Line linha;
        while(linha.read(fileinput)) {
            //se a linha for vazia
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

    // salva o macro nas tabelas MNT e MDT
    //ARRUMAR
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

    // changes macro caller for its definition ARRUMAR
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
