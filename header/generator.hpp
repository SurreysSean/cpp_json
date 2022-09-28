#ifndef GENERATOR_HPP
#define GENERATOR_HPP
#include "content.hpp"

class jsonGenerator
{
    string res;
    string format_res;

protected:
    void gen_array(const jsonContent &);
    void gen_object(const jsonContent &);

public:
    void generate(const jsonContent &);
    void format();
    string getResult() const { return res; }
    string getFormatResult()
    {
        format();
        return format_res;
    }
};

#endif