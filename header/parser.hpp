#ifndef PARSER_HPP
#define PARSER_HPP
#include "content.hpp"
#include "tools.hpp"
using std::string;

class jsonParser
{
    string text;
    int prs_i;
    jsonContent res;

protected:
    void parse_whitespace();
    parseResult parse_null();
    parseResult parse_false();
    parseResult parse_true();
    parseResult parse_value();

public:
    parseResult parse(const string &);
    jsonContent *getResult() { return &res; }
    void clear();
};

#endif