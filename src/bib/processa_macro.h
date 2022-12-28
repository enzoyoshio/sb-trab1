// processa macro aqui
#pragma once
#include <string> 
#include <iostream>
#include <vector>
#include <map>

namespace processa_macro {
    void print();
    void processa(std::string filename);
    void flush_macro(std::ifstream &inputfile, 
                     std::ofstream &outputfile, 
                     std::map<std::string, int> MNT,
        std::map<int, std::vector<std::vector<std::string>>> MDT);
    void save_macro(std::ifstream &fileinput, 
                     std::map<std::string, int> &MNT,
        std::map<int, std::vector<std::vector<std::string>>> &MDT, 
                     int &counter,
                     std::string line);
}
