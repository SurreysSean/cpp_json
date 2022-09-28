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

string encode_utf8(unsigned u)
{
    string utf8_char;
    if (u <= 0x7F)
        utf8_char += (u & 0xFF);
    else if (u <= 0x7FF)
    {
        utf8_char += 0xC0 | ((u >> 6) & 0xFF);
        utf8_char += 0x80 | (u & 0x3F);
    }
    else if (u <= 0xFFFF)
    {
        utf8_char += 0xE0 | ((u >> 12) & 0xFF);
        utf8_char += 0x80 | ((u >> 6) & 0x3F);
        utf8_char += 0x80 | (u & 0x3F);
    }
    else
    {
        utf8_char += 0xF0 | ((u >> 18) & 0xFF);
        utf8_char += 0x80 | ((u >> 12) & 0x3F);
        utf8_char += 0x80 | ((u >> 6) & 0x3F);
        utf8_char += 0x80 | (u & 0x3F);
    }
    return utf8_char;
}

string multiIndex(const int x)
{
    string tmp;
    for (int i = 0; i < x; ++i)
        tmp += oneIndex;
    return tmp;
}
