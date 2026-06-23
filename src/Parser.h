#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include <sstream>

class Parser {
public:
    static std::vector<std::string> tokenize(const std::string& line);
};

#endif