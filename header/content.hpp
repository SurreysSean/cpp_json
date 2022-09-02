#ifndef CONTENT_HPP
#define CONTENT_HPP

#include <string>
enum jsonType
{
    json_null,
    json_true,
    json_false
};

class jsonContent
{
    jsonType type;

public:
    jsonContent() : type(json_null) {}
    void setType(const jsonType t) { type = t; }
    jsonType getType() const { return type; }
    void clear() { type = json_null; }
};

enum parseResult
{
    PARSE_OK,
    PARSE_EXPECT_VALUE,
    PARSE_INVALID_VALUE,
    PARSE_ROOT_NOT_SINGULAR
};

#endif