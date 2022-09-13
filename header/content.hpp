#ifndef CONTENT_HPP
#define CONTENT_HPP

#include <string>
#include <vector>
#include <ostream>

using std::ostream;
using std::string;
using std::vector;

enum jsonType
{
    json_null,
    json_true,
    json_false,
    json_number,
    json_string,
    json_array
};

class jsonContent
{
    jsonType type;
    union valUnionPtr {
        double *num;
        std::string *str;
        vector<jsonContent> *arr;
    } vp;

public:
    jsonContent() : type(json_null) {}
    jsonContent(const jsonContent &);
    ~jsonContent() { clear(); }

    jsonContent &operator=(const jsonContent &);
    friend bool operator==(const jsonContent &, const jsonContent &);
    friend ostream &operator<<(ostream &, const jsonContent &);

    void setType(const jsonType t) { type = t; }
    void setNumber(const double d);
    void setString(const string &s);
    void setArray(const vector<jsonContent> &a);

    jsonType getType() const { return type; }
    double &getNum() const { return *vp.num; }
    string &getStr() const { return *vp.str; }
    vector<jsonContent> &getArr() const { return *vp.arr; }
    void clear();
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
    PARSE_INVALID_UNICODE_SURROGATE,
    PARSE_MISS_COMMA_OR_SQUARE_BRACKET
};

#endif