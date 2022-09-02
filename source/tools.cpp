#include "tools.hpp"

bool is_whitespace(const char &c)
{
    if (c == '\t' || c == '\n' || c == '\r' || c == ' ')
        return true;
    return false;
}
