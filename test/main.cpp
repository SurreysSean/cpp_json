#include <iostream>
#include <string>
#include <climits>

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

void expect_eq_double(double expect, double test, int callerLine)
{
    expect_eq_base(expect == test, test, expect, callerLine);
}

void test_number(double expect, const string &jsonText, int callerLine)
{
    jsonParser parser;
    expect_eq_int(PARSE_OK, parser.parse(jsonText), callerLine);
    expect_eq_int(json_number, parser.getResult()->getType(), callerLine);
    expect_eq_double(expect, parser.getResult()->getNumber(), callerLine);
}

void test_error(int expect, const string &text, int callerLine)
{
    jsonParser parser;
    expect_eq_int(expect, parser.parse(text), callerLine);
    expect_eq_int(json_null, parser.getResult()->getType(), callerLine);
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
    expect_eq_int(PARSE_OK, parser.parse("false"), __LINE__);
    expect_eq_int(json_false, parser.getResult()->getType(), __LINE__);
}

void test_parse_number()
{
    test_number(0.0, "0", __LINE__);
    test_number(0.0, "-0", __LINE__);
    test_number(0.0, "-0.0", __LINE__);
    test_number(1.0, "1", __LINE__);
    test_number(-1.0, "-1", __LINE__);
    test_number(1.5, "1.5", __LINE__);
    test_number(-1.5, "-1.5", __LINE__);
    test_number(3.1416, "3.1416", __LINE__);
    test_number(1E10, "1E10", __LINE__);
    test_number(1e10, "1e10", __LINE__);
    test_number(1E+10, "1E+10", __LINE__);
    test_number(1E-10, "1E-10", __LINE__);
    test_number(-1E10, "-1E10", __LINE__);
    test_number(-1e10, "-1e10", __LINE__);
    test_number(-1E+10, "-1E+10", __LINE__);
    test_number(-1E-10, "-1E-10", __LINE__);
    test_number(1.234E+10, "1.234E+10", __LINE__);
    test_number(1.234E-10, "1.234E-10", __LINE__);

    test_number(2.2250738585072014e-308, "2.2250738585072014e-308", __LINE__); /* Min normal positive double */
    test_number(-2.2250738585072014e-308, "-2.2250738585072014e-308", __LINE__);
    test_number(1.7976931348623157e+308, "1.7976931348623157e+308", __LINE__); /* Max double */
    test_number(-1.7976931348623157e+308, "-1.7976931348623157e+308", __LINE__);
    test_number(1.0000000000000002, "1.0000000000000002", __LINE__);
}

void test_parse_expect_value()
{
    test_error(PARSE_EXPECT_VALUE, "", __LINE__);
    test_error(PARSE_EXPECT_VALUE, " ", __LINE__);
}

void test_parse_invalid_value()
{

    test_error(PARSE_INVALID_VALUE, "nul", __LINE__);
    test_error(PARSE_INVALID_VALUE, "?", __LINE__);

    // invalid number
    test_error(PARSE_INVALID_VALUE, "+0", __LINE__);
    test_error(PARSE_INVALID_VALUE, "+1", __LINE__);
    test_error(PARSE_INVALID_VALUE, ".123", __LINE__); /* at least one digit before '.' */
    test_error(PARSE_INVALID_VALUE, "1.", __LINE__);   /* at least one digit after '.' */
    test_error(PARSE_INVALID_VALUE, "INF", __LINE__);
    test_error(PARSE_INVALID_VALUE, "inf", __LINE__);
    test_error(PARSE_INVALID_VALUE, "NAN", __LINE__);
    test_error(PARSE_INVALID_VALUE, "nan", __LINE__);
}

void test_parse_root_not_singular()
{
    test_error(PARSE_ROOT_NOT_SINGULAR, "null x", __LINE__);
}

void test_parse_number_too_big()
{
    test_error(PARSE_NUMBER_TOO_BIG, "1e-10000", __LINE__);                /* must underflow */
    test_error(PARSE_NUMBER_TOO_BIG, "4.9406564584124654e-324", __LINE__); /* minimum denormal */
    test_error(PARSE_NUMBER_TOO_BIG, "-4.9406564584124654e-324", __LINE__);
    test_error(PARSE_NUMBER_TOO_BIG, "2.2250738585072009e-308", __LINE__); /* Max subnormal double */
    test_error(PARSE_NUMBER_TOO_BIG, "-2.2250738585072009e-308", __LINE__);
    test_error(PARSE_NUMBER_TOO_BIG, "1e309", __LINE__);
    test_error(PARSE_NUMBER_TOO_BIG, "-1e309", __LINE__);
}

void test_parser()
{
    test_parse_null();
    test_parse_true();
    test_parse_false();
    test_parse_expect_value();
    test_parse_invalid_value();
    test_parse_root_not_singular();
    test_parse_number();
    test_parse_number_too_big();
}
int main()
{
    test_parser();
    printf("total %d, pass %d\n", test_count, pass_count);
    return 0;
}