#include "parser.hpp"
#include <stdexcept>

void jsonParser::parse_whitespace()
{
    while (is_whitespace(text[prs_i]))
    {
        ++prs_i;
    }
}

parseResult jsonParser::parse_literals(const string &expectStr, jsonType expectType)
{
    // EXPECT(text[prs_i], expectStr[0]);
    const size_t len = expectStr.size();
    if (text.substr(prs_i, len) != expectStr)
    {
        return PARSE_INVALID_VALUE;
    }
    prs_i += len;
    res.setType(expectType);
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
    }
    catch (const std::out_of_range &e)
    {
        return PARSE_NUMBER_TOO_BIG;
    }
    return PARSE_OK;
}

parseResult jsonParser::parse_string_raw(string &str_res)
{
    // EXPECT('\"', text[prs_i]);
    str_res.clear();
    ++prs_i;
    for (; prs_i != text.size(); ++prs_i)
    {
        switch (text[prs_i])
        {
        case '\"':
            ++prs_i;
            return PARSE_OK;
        case '\\':
            ++prs_i;
            switch (text[prs_i])
            {
            case '\\':
                str_res += '\\';
                break;
            case '\"':
                str_res += '\"';
                break;
            case '/':
                str_res += '/';
                break;
            case 'r':
                str_res += '\r';
                break;
            case 'b':
                str_res += '\b';
                break;
            case 'n':
                str_res += '\n';
                break;
            case 'f':
                str_res += '\f';
                break;
            case 't':
                str_res += '\t';
                break;
            case 'u':
                unsigned u, u1;
                u = parse_hex4();
                if (u > 0xFFFF)
                    return PARSE_INVALID_UNICODE_HEX;
                else if (u < 0x20)
                    return PARSE_INVALID_STRING_CHAR;
                else if (u >= 0xD800 && u <= 0xDBFF)
                {
                    ++prs_i;
                    if (text[prs_i] != '\\' || text[prs_i + 1] != 'u')
                        return PARSE_INVALID_UNICODE_SURROGATE;
                    ++prs_i;
                    u1 = parse_hex4();
                    if (u1 > 0xFFFF)
                        return PARSE_INVALID_UNICODE_HEX;
                    else if (u1 < 0xDC00 || u1 > 0xDFFF)
                        return PARSE_INVALID_UNICODE_SURROGATE;
                    u = (((u - 0xD800) << 10) | (u1 - 0xDC00)) + 0x10000;
                }
                str_res += encode_utf8(u);
                break;
            default:
                return PARSE_INVALID_STRING_ESCAPE;
            }
            break;

        default:
            if (text[prs_i] < 0x20)
                return PARSE_INVALID_STRING_CHAR;
            str_res += text[prs_i];
        }
    }
    return PARSE_MISS_QUOTATION_MARK;
}

parseResult jsonParser::parse_string()
{
    string resStr;
    parseResult ret;
    if ((ret = parse_string_raw(resStr)) == PARSE_OK)
        res.setString(resStr);
    return ret;
}

unsigned jsonParser::parse_hex4()
{
    ++prs_i;
    unsigned utfRes = 0;
    int i = 0;
    for (; i < 4 && text.size() > i + prs_i; ++i)
    {
        utfRes <<= 4;
        char curChar = text[prs_i + i];
        if (isdigit(curChar))
            utfRes += curChar - '0';
        else if (tolower(curChar) >= 'a' && tolower(curChar) <= 'f')
            utfRes += tolower(curChar) - 'a' + 10;
        else
            break;
    }
    if (i != 4)
        return 0x10000;
    prs_i += 3;
    return utfRes;
}
parseResult jsonParser::parse_array()
{
    // EXPECT('[', text[prs_i]);
    ++prs_i;
    parse_whitespace();
    jsType_Arr vec_jC;
    parseResult ret;

    while (text[prs_i] != ']')
    {
        if ((ret = parse_value()) != PARSE_OK)
        {
            return ret;
        }
        vec_jC.emplace_back(res);
        res.clear();
        parse_whitespace();

        if (text[prs_i] == ',')
        {
            ++prs_i;
            parse_whitespace();
            if (text[prs_i] == ']')
                return PARSE_INVALID_VALUE;
        }
        else if (text[prs_i] != ']')
        {
            return PARSE_MISS_COMMA_OR_SQUARE_BRACKET;
        }
    }
    res.setArray(vec_jC);
    ++prs_i;
    return PARSE_OK;
}

parseResult jsonParser::parse_object()
{
    EXPECT('{', text[prs_i]);
    ++prs_i;
    parse_whitespace();
    jsType_Obj map;
    string tempKey;
    parseResult ret;
    while (text[prs_i] != '}')
    {
        if (text[prs_i] != '\"')
            return PARSE_MISS_KEY;
        if ((ret = parse_string_raw(tempKey)) != PARSE_OK)
            return ret;
        parse_whitespace();

        if (text[prs_i] != ':')
            return PARSE_MISS_COLON;
        ++prs_i;
        parse_whitespace();
        if ((ret = parse_value()) != PARSE_OK)
            return ret;
        map[tempKey] = res;
        res.clear();
        parse_whitespace();

        if (text[prs_i] == ',')
        {
            ++prs_i;
            parse_whitespace();
            if (text[prs_i] == '}')
                return PARSE_INVALID_VALUE;
        }
        else if (text[prs_i] != '}')
        {
            return PARSE_MISS_COMMA_OR_CURLY_BRACKET;
        }
    }

    res.setObject(map);
    ++prs_i;
    return PARSE_OK;
}

parseResult jsonParser::parse_value()
{
    switch (text[prs_i])
    {
    case 'n':
        return parse_literals("null", json_null);
    case 't':
        return parse_literals("true", json_true);
    case 'f':
        return parse_literals("false", json_false);
    case '[':
        return parse_array();
    case '{':
        return parse_object();
    case '\"':
        return parse_string();
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
