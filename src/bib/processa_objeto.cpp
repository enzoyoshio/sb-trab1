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
        //FAZER ERRO LEXICO
        // this->print();
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

        auto& [linecount, mempos, t_inst, t_size, ts, t_dir, mem, err] = m;

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
            auto& [rot, ope, ops, vazio, erro] = linha;

            // se existe algum rotulo
            if(rot != "") {

                // se eu ja tiver definido esse simbolo
                if(ts.find(rot) != ts.end()) {
                    // jogar erro, redefinicao de simbolo
                    // std::cout << "erro, redefinicao do simbolo " << linha.rotulo 
                    // << " na linha " << linecount << std::endl;
                    err.emplace_back("semantico", "redefinicao do simbolo " + rot + 
                            " na linha " + std::to_string(linecount));
                }else { // nao defini ainda, vou definir agora
                    ts[rot] = mempos;
                }
            }

            // verificar a operacao
            // if(ope == "") { 
            //     // caso da pessoa ter dado enter,
            //     // nao sei anida como resolver -> mas nao precisa desse if
            //     // apenas ignora, o endereco na memoria nao muda
            // } // se eu achar essa operacao na tabela de instrucao 
            if(t_inst.find(ope) != t_inst.end()) {
                mempos += t_size[ope];
            }else if(ope != "") {

                // se a operacao for uma diretiva
                if(t_dir.find(ope) != t_dir.end()) {
                    if(ope == "space") {
                        if(ops.size() > 1) {
                            std::cout << "lembra de colocar erro aqui" << std::endl;
                            // adicionar erro, quantidade de parametros muito grande
                            // vou adicionar um erro aqui mas talvez tenha que mudar
                            err.emplace_back("sintatico", "muitos argumentos pro comando space na linha " + std::to_string(linecount));
                        }else if(ops.size() == 1) {
                            mempos += helper::str2num(ops[0]);
                        }else mempos++;
                    } 
                    else if(ope == "const")
                        mempos++;
                    
                    
                }else {
                    std::cout << "operacao " << ope 
                    << " nao identificada na linha " << linecount << std::endl;
                    err.emplace_back("sintatico", "operacao " + ope + " nao identificada na linha " + std::to_string(linecount));
                }
            }
            linecount++;
        }
        fileinput.close();

        

        return m;
    }
    
    void passagem2(std::string filename, Montador m) {
        
        auto& [linecount, mempos, t_inst, t_size, ts, t_dir, mem, err] = m;


        std::cout << "cheguei na passagem2\n";

        mempos = 0;
        linecount = 2;

        std::string finput = filename + ".MCR";
        std::ifstream fileinput(finput);

        if(!fileinput) {
            std::cout << "arquivo nao encontrado :(" << std::endl;
            return;
        }

        Line linha, last;

        linha.read(fileinput);

        if(linha.rotulo != "") {
            m.errors.emplace_back("sintatico", "nao permitido o uso de rotulo fora das sections");
        }

        if(linha.operacao != "section") {
            m.errors.emplace_back("sintatico ou semantico ?", "secao text faltante");
        }

        while(linha.read(fileinput)) {
            auto& [rot, ope, ops, vazio, erro] = linha;

            if(erro) {
                // adicionar os erros lexicos e talz
                // no momento esta pegando so o erro de dois rotulos na mesma linha
                err.emplace_back("sintatico", "nao pode haver dois rotulos na mesma linha");
            }

            // verificar se tem um simbolo fora da tabela de simbolo
            // jogar erro se tiver
            if(ope != "section") {
                for(auto sim: ops) {
                    if(!helper::checkSymbol(sim, ts)) {
                        std::cout << "erro, simbolo " << sim << " nao definido" << std::endl;
                        err.emplace_back("semantico", "simbolo " + sim + " nao definido na linha " + std::to_string(linecount));
                    }
                }
            }

            // checar se eu acho a operacao na tabela de instrucao
            if(t_inst.find(ope) != t_inst.end()) {

                // checar se tem a quantidade certa de operandos
                if(ops.size() != t_size[ope] -1) {
                    std::cout << "quantidade de argumentos invalida na linha " << linecount << std::endl;
                    err.emplace_back("sintatico", "quantidade de argumentos invalida na linha " + std::to_string(linecount));
                }

                // falando na memoria qual o opcode da instrucao
                mem[mempos++] = t_inst[ope];
                // para cada arguemnto eu falo qual o valor deve receber
                for(auto op: ops) {
                    // se for um simbolo eu falo onde na memoria ele esta localizado
                    if(ts.find(op) != ts.end()) 
                        mem[mempos++] = ts[op];
                    else if(helper::checkSymbol(op, ts)) {
                        // se for um numero eu coloco o valor do numero
                        if(helper::isnumber(op)) 
                            mem[mempos++] = helper::str2num(op);
                        else { // se for um vetor do tipo X+2
                            std::vector<std::string> v = helper::parser(op, '+');
                            mem[mempos++] = ts[v[0]] + helper::str2num(v[1]);
                        }
                    } 
                }
            }else {
                // vou procurar se eh uma diretiva agora
                if(t_dir.find(ope) != t_dir.end()) {
                    // se for um space
                    if(ope == "space") {
                        // checar se tem um ou zero argumentos
                        if(ops.size() > 1) {
                            err.emplace_back("sintatico", "numero de argumentos invalido para a diretiva const na linha " + std::to_string(linecount));
                        }else if(ops.empty()) {
                            mem[mempos++] = 0;
                        }else if(helper::isnumber(ops[0])) {
                            for(int i = 0; i < helper::str2num(ops[0]); i++)
                                mem[mempos++] = 0;
                        }else {
                            err.emplace_back("semantico ou lexico", "simbolo invalido na linha " + std::to_string(linecount));
                        }
                    } // se for const
                    else if(ope == "const") {
                        if(ops.size() != 1) {
                            err.emplace_back("sintatico", "numero de argumentos invalido para a diretiva const na linha " + std::to_string(linecount));
                        }else if(helper::isnumber(ops[0])) {
                            mem[mempos++] = helper::str2num(ops[0]);
                        }else {
                            err.emplace_back("semantico ou lexico", "simbolo invalido na linha " + std::to_string(linecount));
                        }
                    }

                    if(ope == "section" && last.rotulo != "" && last.operacao == "") {
                        err.emplace_back("semantico", "rotulo vazio na linha " + std::to_string(linecount-1));
                    }
                }else if(ope != "") {
                    std::cout << "operacao nao identificada" << std::endl;
                    err.emplace_back("sintatico", "o comando " + ope + " na linha " 
                                    + std::to_string(linecount) + " nao existe");
                }
            }

            linecount++;
            last = linha;
        }
        // so tem erro de rotulo numa linha e definicao em outra
        // se for um rotulo vazio no final ne?
        if(linha.rotulo != "") {
            err.emplace_back("semantico", "rotulo vazio na linha " + std::to_string(linecount));
        }

        // depois ver de refatorar isso 
        std::ofstream fileoutput(filename + ".OBJ");
        while(m.memory.back() == -1) m.memory.pop_back(); // gambiarra pra dar certo
        for(int i = 0; i < m.memory.size(); i++) if(m.memory[i] == -1) m.memory[i] = 0; // mais gambiarra
        
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
