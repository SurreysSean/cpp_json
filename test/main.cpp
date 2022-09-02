#include <iostream>
#include <string>

#include "parser.hpp"

int pass_count = 0;
int test_count = 0;

using std::string;
template <typename T>
void expect_eq_base(bool equality, T test, T expect, int callerLine)
{
    ++test_count;
    if (equality)
        ++pass_count;
    else
        std::cerr << __FILE__ << ": " << callerLine << ": expect: " << expect << ",actual: " << test << "\n";
}

void expect_eq_int(int expect, int test, int callerLine)
{
    expect_eq_base(test == expect, test, expect, callerLine);
}

void test_parse_null()
{
    jsonParser parser;
    expect_eq_int(PARSE_OK, parser.parse("null"), __LINE__);
    expect_eq_int(json_null, parser.getResult()->getType(), __LINE__);
}

void test_parse_true()
{
    jsonParser parser;
    expect_eq_int(PARSE_OK, parser.parse("true"), __LINE__);
    expect_eq_int(json_true, parser.getResult()->getType(), __LINE__);
}

void test_parse_false()
{
    jsonParser parser;
    expect_eq_int(PARSE_INVALID_VALUE, parser.parse("false"), __LINE__);
    expect_eq_int(json_false, parser.getResult()->getType(), __LINE__);
}

void test_parse_expect_value()
{
    jsonParser parser;
    expect_eq_int(PARSE_EXPECT_VALUE, parser.parse(""), __LINE__);
    expect_eq_int(json_null, parser.getResult()->getType(), __LINE__);

    parser.clear();
    expect_eq_int(PARSE_EXPECT_VALUE, parser.parse(" "), __LINE__);
    expect_eq_int(json_null, parser.getResult()->getType(), __LINE__);
}

void test_parse_invalid_value()
{
    jsonParser parser;
    expect_eq_int(PARSE_INVALID_VALUE, parser.parse("nul"), __LINE__);
    expect_eq_int(json_null, parser.getResult()->getType(), __LINE__);

    parser.clear();
    expect_eq_int(PARSE_INVALID_VALUE, parser.parse("?"), __LINE__);
    expect_eq_int(json_null, parser.getResult()->getType(), __LINE__);
}

void test_parse_root_not_singular()
{
    jsonParser parser;
    expect_eq_int(PARSE_ROOT_NOT_SINGULAR, parser.parse("null x"), __LINE__);
    expect_eq_int(json_null, parser.getResult()->getType(), __LINE__);
}

void test_parser()
{
    test_parse_null();
    test_parse_true();
    test_parse_false();
    test_parse_expect_value();
    test_parse_invalid_value();
    test_parse_root_not_singular();
}
int main()
{
    test_parser();
    printf("total %d, pass %d\n", test_count, pass_count);
    return 0;
}