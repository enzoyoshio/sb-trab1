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

        }
        fileoutput << '\n';
    }

    bool Line::empty() {
        return this->vazio;
    }

    Line::Line() {
        this->erro = 0;
        this->operacao = "";
        this->operadores = {};
        this->rotulo = "";
        this->vazio = 0;
    }

    Line::Line(std::string s) {
        *this = Line();

        s = helper::join(helper::remove_comments(s), ' ');

        if(s.empty() || s == "") {
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
        }else { // caso tenha label
            tok = helper::parser(s, ':');
            this->rotulo = tok[0]; // pega o primeiro e rotula

            // se tiver mais de duas definicoes
            if(tok.size() > 2) this->erro = true; 
            
            // se nao for um rotulo sozinho
            if(tok.size() > 1) 
                tok = helper::parser(tok[1], ' ');
            else tok = {}; // aqui eh um rotulo sozinho
        }
        
        if(tok.size()) {
            // a operacao vai ser a primeira
            this->operacao = tok[0];
            tok.erase(tok.begin());
            // vou levar aluns pontos em consideracao
            if(this->operacao == "copy") {
                tok = helper::parser(helper::join(tok, ' '), ',');
            }else {
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

        auto& [linecount, mempos, tabelaInstrucao, tabelaTamanho, tabelaSimbolo, tabelaDiretivas, memoria, erros] = m;

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
            auto& [rotulo, operadores, operacao, vazio, erro] = linha;

            // se existe algum rotulo
            if(rotulo != "") {
                // se eu ja tiver definido esse simbolo
                if(tabelaSimbolo.find(rotulo) != tabelaSimbolo.end()) {
                    // jogar erro, redefinicao de simbolo
                    erros.emplace_back("semantico", "redefinicao do simbolo " + rotulo + 
                            " na linha " + std::to_string(linecount));
                }else { // nao defini ainda, vou definir agora
                    tabelaSimbolo[rotulo] = mempos;
                }
                
            }

            // verificar a operacao
            // if(operadores == "") { 
            //     // caso da pessoa ter dado enter,
            //     // nao sei anida como resolver -> mas nao precisa desse if
            //     // apenas ignora, o endereco na memoria nao muda
            // } // se eu achar essa operacao na tabela de instrucao 
            if(tabelaInstrucao.find(operadores) != tabelaInstrucao.end()) {
                mempos += tabelaTamanho[operadores];
            }else if(operadores != "") {

                // se a operacao for uma diretiva
                if(tabelaDiretivas.find(operadores) != tabelaDiretivas.end()) {
                    if(operadores == "space") {
                        if(operacao.size() > 1) {
                            std::cout << "lembra de colocar erro aqui" << std::endl;
                            // adicionar erro, quantidade de parametros muito grande
                            // vou adicionar um erro aqui mas talvez tenha que mudar
                            erros.emplace_back("sintatico", "muitos argumentos pro comando space na linha " + std::to_string(linecount));
                        }else if(operacao.size() == 1) {
                            mempos += helper::str2num(operacao[0]);
                        }else mempos++;
                    } 
                    else if(operadores == "const")
                        mempos++;
                    
                    
                }else {
                    std::cout << "operacao " << operadores 
                    << " nao identificada na linha " << linecount << std::endl;
                    erros.emplace_back("sintatico", "operacao " + operadores + " nao identificada na linha " + std::to_string(linecount));
                }
            }
            linecount++;
        }
        fileinput.close();

        

        return m;
    }
    
    void passagem2(std::string filename, Montador m) {
        // linecount = contador de linha
        //mempos =  posicao da memoria
        //o resto das variaveis sao bem auto explicativas
        auto& [linecount, mempos, tabelaInstrucao, tabelaTamanho, tabelaSimbolo, tabelaDiretivas, memoria, erros] = m;


        //std::cout << "cheguei na passagem2\n";

        mempos = 0;
        linecount = 2;
        //pega o nome do arquivo e adiciona a extencao
        std::string finput = filename + ".MCR";
        std::ifstream fileinput(finput);

        //se nao houver arquivo
        if(!fileinput) {
            std::cout << "arquivo nao encontrado :(" << std::endl;
            return;
        }

        Line linha, last;

        //le o arquivo
        linha.read(fileinput);

        //se houver rotulo
        if(linha.rotulo != "") {
            m.errors.emplace_back("sintatico", "nao permitido o uso de rotulo fora das sections");
        }

        //tem q ARRUMAR e falar se é sintatico ou semântico 
        if(linha.operacao != "section") {
            m.errors.emplace_back("sintatico ou semantico ?", "secao text faltante");
        }

        //equanto tiver arquivo para ler
        while(linha.read(fileinput)) {

            //decalara as variaveis baseado  nos valores lidos na linha
            auto& [rotulo, operadores, operacao, vazio, erro] = linha;

            if(erro) {
                // no momento esta pegando so o erro de dois rotulos na mesma linha
                erros.emplace_back("sintatico", "nao pode haver dois rotulos na mesma linha");
            }

            if(operadores != "section") {
                for(auto simbolo: operacao) {
                    bool eh_simbolo = helper::checkSymbol(simbolo, tabelaSimbolo);
                    if(simbolo[0] >= '0' and simbolo[0] <= '9' and !eh_simbolo){// verifica se o rotulo comeca com numero
                        erros.emplace_back("lexico", "simbolo comeca com numero, o simbolo " + simbolo +
                        " na linha " + std::to_string(linecount));
                    }else if(helper::checkSim(simbolo) and !eh_simbolo){
                        //verifica se o Simbulo possui algum caracter especial
                        erros.emplace_back("lexico", "uso invalido de caracteres no simbolo " + simbolo + 
                        " na linha " + std::to_string(linecount));
                    }else if(!eh_simbolo) {
                        // verificar se tem um simbolo fora da tabela de simbolo
                        // jogar erro se tiver
                        std::cout << "erro, simbolo " << simbolo << " nao definido" << std::endl;
                        erros.emplace_back("semantico", "simbolo " + simbolo + " nao definido na linha " + std::to_string(linecount));
                    }
                    
                    
                }
            }

            // checar se existe a operacao na tabela de instrucao
            if(tabelaInstrucao.find(operadores) != tabelaInstrucao.end()) {

                // checar se tem a quantidade certa de operandos
                if(operacao.size() != tabelaTamanho[operadores] -1) {
                    //std::cout << "quantidade de argumentos invalida na linha " << linecount << std::endl;
                    erros.emplace_back("sintatico", "quantidade de argumentos invalida na linha " + std::to_string(linecount));
                }

                //qual o opcode da instrucao na memoria
                memoria[mempos++] = tabelaInstrucao[operadores];
                // para cada arguemnto determina qual o valor deve receber
                for(auto op: operacao) {
                    // se for um simbolo mostra onde na memoria ele esta localizado
                    if(tabelaSimbolo.find(op) != tabelaSimbolo.end()) 
                        memoria[mempos++] = tabelaSimbolo[op];
                    else if(helper::checkSymbol(op, tabelaSimbolo)) {
                        // se for um numero eu coloca o valor do numero
                        if(helper::isnumber(op)) 
                            memoria[mempos++] = helper::str2num(op);
                        else { // se for um vetor do tipo X+2
                            std::vector<std::string> v = helper::parser(op, '+');
                            memoria[mempos++] = tabelaSimbolo[v[0]] + helper::str2num(v[1]);
                        }
                    } 
                }
            }else {
                //determina se eh uma diretiva
                if(tabelaDiretivas.find(operadores) != tabelaDiretivas.end()) {
                    // se for um space
                    if(operadores == "space") {
                        // checar se tem um ou zero argumentos
                        if(operacao.size() > 1) {
                            erros.emplace_back("sintatico", "numero de argumentos invalido para a diretiva const na linha " + std::to_string(linecount));
                        }else if(operacao.empty()) {
                            memoria[mempos++] = 0;
                        }else if(helper::isnumber(operacao[0])) {//checar se eh um numero
                            for(int i = 0; i < helper::str2num(operacao[0]); i++)
                                memoria[mempos++] = 0;
                        }else {//se todos os casos falharem da erro
                        //ARRUMAR determinar se é erro lexico ou sintatico
                            erros.emplace_back("semantico ou lexico", "simbolo invalido na linha " + std::to_string(linecount));
                        }
                    } // se for const
                    else if(operadores == "const") {
                        if(operacao.size() != 1) {//verifica se possui o numero correto de argumentos
                            erros.emplace_back("sintatico", "numero de argumentos invalido para a diretiva const na linha " + std::to_string(linecount));
                        }else if(helper::isnumber(operacao[0])) {//verifica se o primeiro valor eh um numero
                            memoria[mempos++] = helper::str2num(operacao[0]);
                        }else {
                            //ARRUMAR determinar se é erro lexico ou sintatico
                            erros.emplace_back("semantico ou lexico", "simbolo invalido na linha " + std::to_string(linecount));
                        }
                    }
                    //verifica se o rotulo esta vazio
                    if(operadores == "section" && last.rotulo != "" && last.operacao == "") {
                        erros.emplace_back("semantico", "rotulo vazio na linha " + std::to_string(linecount-1));
                    }
                }else if(operadores != "") {
                    //verifica se a operacao  existe
                    std::cout << "operacao nao identificada" << std::endl;
                    erros.emplace_back("sintatico", "o comando " + operadores + " na linha " 
                                    + std::to_string(linecount) + " nao existe");
                }
            }

            linecount++;
            last = linha;
        }
        //verifica se o rotulo esta vazio
        if(linha.rotulo != "") {
            erros.emplace_back("semantico", "rotulo vazio na linha " + std::to_string(linecount));
        }

        // depois ver de refatorar isso 
        std::ofstream fileoutput(filename + ".OBJ");
        while(m.memory.back() == -1) m.memory.pop_back(); // Enquanto houver memoria
        for(int i = 0; i < m.memory.size(); i++) if(m.memory[i] == -1) m.memory[i] = 0; // limpa a memoria? ARRUMAR
        
        //se houver erros mostra eles. Se nao houver, compila o codigo? ARRUMAR
        if(m.errors.size())
            m.print_error();
        else
            helper::flushline<short>(fileoutput, m.memory);
    
        fileoutput.close();

    }

    void Montador::print_error() {
        for(auto [type, msg]: this->errors) {
            std::cout << "\n\nerro do tipo " << type << std::endl;
            std::cout << msg << std::endl;
        } 
    }

}
