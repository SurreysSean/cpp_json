#include "parser.hpp"
#include <iostream>
#include <stdexcept>

void jsonParser::parse_whitespace()
{
    while (is_whitespace(text[prs_i]))
    {
        ++prs_i;
    }
}

parseResult jsonParser::parse_null()
{
    EXPECT('n', text[prs_i]);
    if (text.substr(prs_i, 4) != "null")
    {
        return PARSE_INVALID_VALUE;
    }
    prs_i += 4;
    res.setType(json_null);
    return PARSE_OK;
}

parseResult jsonParser::parse_false()
{

    EXPECT('f', text[prs_i]);
    if (text.substr(prs_i, 5) != "false")
    {
        return PARSE_INVALID_VALUE;
    }
    prs_i += 5;
    res.setType(json_false);
    return PARSE_OK;
}

parseResult jsonParser::parse_true()
{
    EXPECT('t', text[prs_i]);
    if (text.substr(prs_i, 4) != "true")
    {
        return PARSE_INVALID_VALUE;
    }
    prs_i += 4;
    res.setType(json_true);
    return PARSE_OK;
}

parseResult jsonParser::parse_number()
{
    int start = prs_i;
    if (text[prs_i] == '-')
        ++prs_i;
    if (text[prs_i] == '0')
        ++prs_i;
    else
    {
        if (!is_digit1to9(text[prs_i]))
            return PARSE_INVALID_VALUE;
        for (++prs_i; isdigit(text[prs_i]); ++prs_i)
            ;
    }

    if (text[prs_i] == '.')
    {
        ++prs_i;
        if (!isdigit(text[prs_i]))
            return PARSE_INVALID_VALUE;
        for (++prs_i; isdigit(text[prs_i]); ++prs_i)
            ;
    }

    if (text[prs_i] == 'e' || text[prs_i] == 'E')
    {
        ++prs_i;
        if (text[prs_i] == '-' || text[prs_i] == '+')
            ++prs_i;
        if (!isdigit(text[prs_i]))
            return PARSE_INVALID_VALUE;
        for (++prs_i; isdigit(text[prs_i]); ++prs_i)
            ;
    }

    try
    {
        // 非规范的浮点数也会被判定为errno：34

        res.setNumber(stod(text.substr(start, prs_i - start)));
        res.setType(json_number);
    }
    catch (const std::out_of_range &e)
    {
        return PARSE_NUMBER_TOO_BIG;
    }
    return PARSE_OK;
}

parseResult jsonParser::parse_value()
{
    switch (text[prs_i])
    {
    case 'n':
        return parse_null();
    case 't':
        return parse_true();
    case 'f':
        return parse_false();
    case '\0':
        return PARSE_EXPECT_VALUE;
    default:
        return parse_number();
    }
}

parseResult jsonParser::parse(const string &s)
{
    text = s;
    prs_i = 0;

    parseResult ret;
    parse_whitespace();
    if ((ret = parse_value()) == PARSE_OK)
    {
        parse_whitespace();
        if (text[prs_i] != '\0')
        {
            clear();
            return PARSE_ROOT_NOT_SINGULAR;
        }
    }
    return ret;
}

void jsonParser::clear()
{
    text.clear();
    res.clear();
    prs_i = 0;
}
