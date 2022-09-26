#ifndef PARSER_HPP
#define PARSER_HPP
#include "content.hpp"
#include "tools.hpp"

class jsonParser
{
    string text;
    int prs_i;
    jsonContent res;

    unsigned parse_hex4();
    parseResult parse_string_raw(string &);

protected:
    void parse_whitespace();
    parseResult parse_literals(const string &, jsonType);
    parseResult parse_number();
    parseResult parse_value();
    parseResult parse_string();
    parseResult parse_array();
    parseResult parse_object();

public:
    parseResult parse(const string &);
    jsonContent &getResult() { return res; }
    void clear();
    ~jsonParser() { clear(); }
};

#endif