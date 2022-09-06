#ifndef CONTENT_HPP
#define CONTENT_HPP

#include <string>
enum jsonType
{
    json_null,
    json_true,
    json_false,
    json_number
};

class jsonContent
{
    jsonType type;
    double number;

public:
    jsonContent() : type(json_null) {}
    void setType(const jsonType t) { type = t; }
    jsonType getType() const { return type; }
    void setNumber(const double d) { number = d; }
    double getNumber() const { return number; }
    void clear() { type = json_null; }
};

enum parseResult
{
    PARSE_OK,
    PARSE_EXPECT_VALUE,
    PARSE_INVALID_VALUE,
    PARSE_ROOT_NOT_SINGULAR,
    PARSE_NUMBER_TOO_BIG
};

#endif