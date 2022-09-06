#include "tools.hpp"

bool is_whitespace(const char &c)
{
    if (c == '\t' || c == '\n' || c == '\r' || c == ' ')
        return true;
    return false;
}

bool is_digit1to9(const char &c)
{
    return (isdigit(c) && c != '0');
}
