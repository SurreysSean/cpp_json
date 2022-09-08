#ifndef CONTENT_HPP
#define CONTENT_HPP

#include <string>
using std::string;
enum jsonType
{
    json_null,
    json_true,
    json_false,
    json_number,
    json_string
};

class jsonContent
{
    jsonType type;
    union valUnionPtr {
        double *num;
        std::string *str;
    } vp;

public:
    jsonContent() : type(json_null) {}
    ~jsonContent() { clear(); }

    void setType(const jsonType t) { type = t; }
    void setNumber(const double d)
    {
        vp.num = new double(d);
        setType(json_number);
    }
    void setString(const string &s)
    {
        vp.str = new string(s);
        setType(json_string);
    }

    jsonType getType() const { return type; }
    double getNum() const { return *vp.num; }
    string getStr() const { return *vp.str; }
    inline void clear();
};

enum parseResult
{
    PARSE_OK = 0,
    PARSE_EXPECT_VALUE,
    PARSE_INVALID_VALUE,
    PARSE_ROOT_NOT_SINGULAR,
    PARSE_NUMBER_TOO_BIG,
    PARSE_MISS_QUOTATION_MARK,
    PARSE_INVALID_STRING_ESCAPE,
    PARSE_INVALID_STRING_CHAR,
    PARSE_INVALID_UNICODE_HEX,
    PARSE_INVALID_UNICODE_SURROGATE
};

void jsonContent::clear()
{
    if (type == json_number && vp.num)
    {
        delete vp.num;
    }
    else if (type == json_string && vp.str)
    {
        delete vp.str;
    }

    type = json_null;
}

#endif