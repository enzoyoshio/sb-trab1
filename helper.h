#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

namespace helper {

    std::vector<std::string> get_tokens(std::ifstream &file);
    std::vector<std::string> parser(std::string s, char c);
    std::vector<std::string> remove_comments(std::vector<std::string> v);
}
