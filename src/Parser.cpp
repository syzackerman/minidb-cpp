#include "Parser.h"

std::vector<std::string> Parser::tokenize(const std::string& line){
    std::stringstream ss(line);
    std::vector<std::string> tokens;
    std::string token;

    while(ss >> token){
        tokens.push_back(token);
    }

    return tokens;
}