#ifndef CONTENT_HPP
#define CONTENT_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <ostream>

using std::ostream;
using std::string;
using std::unordered_map;
using std::vector;

class jsonContent;
using jsType_Obj = unordered_map<string, jsonContent>;
using jsType_Arr = vector<jsonContent>;

enum jsonType
{
    json_null,
    json_true,
    json_false,
    json_number,
    json_string,
    json_array,
    json_object
};

class jsonContent
{
    jsonType type;
    union valUnionPtr {
        double *num;
        string *str;
        jsType_Arr *arr;
        jsType_Obj *obj;
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
    void setArray(const jsType_Arr &a);
    void setObject(const jsType_Obj &);

    jsonType getType() const { return type; }
    double &getNum() const { return *vp.num; }
    string &getStr() const { return *vp.str; }
    jsType_Arr &getArr() const { return *vp.arr; }
    jsType_Obj &getObj() const { return *vp.obj; }
    void clear();
};

enum parseResult
{
    PARSE_OK = 0,
    PARSE_EXPECT_VALUE,
    PARSE_INVALID_VALUE,
    PARSE_ROOT_NOT_SINGULAR,
    PARSE_NUMBER_TOO_BIG,

    // String error
    PARSE_MISS_QUOTATION_MARK,
    PARSE_INVALID_STRING_ESCAPE,
    PARSE_INVALID_STRING_CHAR,
    PARSE_INVALID_UNICODE_HEX,
    PARSE_INVALID_UNICODE_SURROGATE,

    //Array error
    PARSE_MISS_COMMA_OR_SQUARE_BRACKET,

    //Object error
    PARSE_MISS_KEY,
    PARSE_MISS_COLON,
    PARSE_MISS_COMMA_OR_CURLY_BRACKET
};

#endif