#include <iostream>
#include <string>
#include <cstring>
#include <climits>

#include "parser.hpp"

int pass_count = 0;
int test_count = 0;

using std::string;
template <typename T>
void expect_eq(const T &expect, const T &test, int callerLine)
{
    ++test_count;
    if (test == expect)
        ++pass_count;
    else
        std::cerr << __FILE__ << " : line " << callerLine << " :\n\texpect: " << expect << ",actual: " << test << "\n";
}

void test_number(const double expect, const string &jsonText, int callerLine)
{
    jsonParser parser;
    expect_eq(PARSE_OK, parser.parse(jsonText), callerLine);
    expect_eq(json_number, parser.getResult().getType(), callerLine);
    expect_eq(expect, parser.getResult().getNum(), callerLine);
}

void test_string(const string &expect, const string &jsonText, int callerLine)
{

    jsonParser parser;
    expect_eq(PARSE_OK, parser.parse(jsonText), callerLine);
    expect_eq(json_string, parser.getResult().getType(), callerLine);
    expect_eq(expect, parser.getResult().getStr(), callerLine);
}
void test_error(const parseResult expect, const string &text, int callerLine)
{
    jsonParser parser;
    expect_eq(expect, parser.parse(text), callerLine);
    expect_eq(json_null, parser.getResult().getType(), callerLine);
}

void test_parse_null()
{
    jsonParser parser;
    expect_eq(PARSE_OK, parser.parse("null"), __LINE__);
    expect_eq(json_null, parser.getResult().getType(), __LINE__);
}

void test_parse_true()
{
    jsonParser parser;
    expect_eq(PARSE_OK, parser.parse("true"), __LINE__);
    expect_eq(json_true, parser.getResult().getType(), __LINE__);
}

void test_parse_false()
{
    jsonParser parser;
    expect_eq(PARSE_OK, parser.parse("false"), __LINE__);
    expect_eq(json_false, parser.getResult().getType(), __LINE__);
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

void test_parse_string()
{
    test_string("", "\"\"", __LINE__);
    test_string("Hello", "\"Hello\"", __LINE__);
    test_string("Hello\nWorld", "\"Hello\\nWorld\"", __LINE__);
    test_string("\" \\ / \b \f \n \r \t", "\"\\\" \\\\ \\/ \\b \\f \\n \\r \\t\"", __LINE__);
    test_string("\x24", "\"\\u0024\"", __LINE__);                    /* Dollar sign U+0024 */
    test_string("\xC2\xA2", "\"\\u00A2\"", __LINE__);                /* Cents sign U+00A2 */
    test_string("\xE2\x82\xAC", "\"\\u20AC\"", __LINE__);            /* Euro sign U+20AC */
    test_string("\xF0\x9D\x84\x9E", "\"\\uD834\\uDD1E\"", __LINE__); /* G clef sign U+1D11E */
    test_string("\xF0\x9D\x84\x9E", "\"\\ud834\\udd1e\"", __LINE__); /* G clef sign U+1D11E */
}

void test_parse_array()
{
    jsonParser parser;
    expect_eq(PARSE_OK, parser.parse("[ ]"), __LINE__);
    expect_eq(json_array, parser.getResult().getType(), __LINE__);
    const vector<jsonContent> &vec1 = parser.getResult().getArr();
    expect_eq(true, vec1.empty(), __LINE__);
    parser.clear();

    expect_eq(PARSE_OK, parser.parse("[ null , false , true , 123 , \"abc\" ]"), __LINE__);
    expect_eq(json_array, parser.getResult().getType(), __LINE__);
    const vector<jsonContent> &vec2 = parser.getResult().getArr();
    expect_eq(size_t(5), vec2.size(), __LINE__);
    expect_eq(json_null, vec2[0].getType(), __LINE__);
    expect_eq(json_false, vec2[1].getType(), __LINE__);
    expect_eq(json_true, vec2[2].getType(), __LINE__);
    expect_eq(json_number, vec2[3].getType(), __LINE__);
    expect_eq(123.0, vec2[3].getNum(), __LINE__);
    expect_eq(json_string, vec2[4].getType(), __LINE__);
    expect_eq(string("abc"), vec2[4].getStr(), __LINE__);
    parser.clear();

    expect_eq(PARSE_OK, parser.parse("[ [ ] , [ 0 ] , [ 0 , 1 ] , [ 0 , 1 , 2 ] ]"), __LINE__);
    expect_eq(json_array, parser.getResult().getType(), __LINE__);
    const vector<jsonContent> &vec3 = parser.getResult().getArr();
    expect_eq(size_t(4), vec3.size(), __LINE__);
    for (size_t i = 0; i < vec3.size(); ++i)
    {
        expect_eq(json_array, vec3[i].getType(), __LINE__);
        const vector<jsonContent> &vec3s = vec3[i].getArr();
        expect_eq(size_t(i), vec3s.size(), __LINE__);
        for (size_t j = 0; j < vec3s.size(); ++j)
        {
            expect_eq(json_number, vec3s[j].getType(), __LINE__);
            expect_eq(double(j), vec3s[j].getNum(), __LINE__);
        }
    }
    parser.clear();
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

void test_parse_miss_quotation_mark()
{
    test_error(PARSE_MISS_QUOTATION_MARK, "\"", __LINE__);
    test_error(PARSE_MISS_QUOTATION_MARK, "\"abc", __LINE__);
}

void test_parse_invalid_string_escape()
{
    test_error(PARSE_INVALID_STRING_ESCAPE, "\"\\v\"", __LINE__);
    test_error(PARSE_INVALID_STRING_ESCAPE, "\"\\'\"", __LINE__);
    test_error(PARSE_INVALID_STRING_ESCAPE, "\"\\0\"", __LINE__);
    test_error(PARSE_INVALID_STRING_ESCAPE, "\"\\x12\"", __LINE__);
}

void test_parse_invalid_string_char()
{
    test_error(PARSE_INVALID_STRING_CHAR, "\"\x01\"", __LINE__);
    test_error(PARSE_INVALID_STRING_CHAR, "\"\x1F\"", __LINE__);
}

void test_parse_invalid_unicode_hex()
{
    test_error(PARSE_INVALID_UNICODE_HEX, "\"\\u\"", __LINE__);
    test_error(PARSE_INVALID_UNICODE_HEX, "\"\\u0\"", __LINE__);
    test_error(PARSE_INVALID_UNICODE_HEX, "\"\\u01\"", __LINE__);
    test_error(PARSE_INVALID_UNICODE_HEX, "\"\\u012\"", __LINE__);
    test_error(PARSE_INVALID_UNICODE_HEX, "\"\\u/000\"", __LINE__);
    test_error(PARSE_INVALID_UNICODE_HEX, "\"\\uG000\"", __LINE__);
    test_error(PARSE_INVALID_UNICODE_HEX, "\"\\u0/00\"", __LINE__);
    test_error(PARSE_INVALID_UNICODE_HEX, "\"\\u0G00\"", __LINE__);
    test_error(PARSE_INVALID_UNICODE_HEX, "\"\\u00/0\"", __LINE__);
    test_error(PARSE_INVALID_UNICODE_HEX, "\"\\u00G0\"", __LINE__);
    test_error(PARSE_INVALID_UNICODE_HEX, "\"\\u000/\"", __LINE__);
    test_error(PARSE_INVALID_UNICODE_HEX, "\"\\u000G\"", __LINE__);
    test_error(PARSE_INVALID_UNICODE_HEX, "\"\\u 123\"", __LINE__);
}

void test_parse_invalid_unicode_surrogate()
{
    test_error(PARSE_INVALID_UNICODE_SURROGATE, "\"\\uD800\"", __LINE__);
    test_error(PARSE_INVALID_UNICODE_SURROGATE, "\"\\uDBFF\"", __LINE__);
    test_error(PARSE_INVALID_UNICODE_SURROGATE, "\"\\uD800\\\\\"", __LINE__);
    test_error(PARSE_INVALID_UNICODE_SURROGATE, "\"\\uD800\\uDBFF\"", __LINE__);
    test_error(PARSE_INVALID_UNICODE_SURROGATE, "\"\\uD800\\uE000\"", __LINE__);
}

void test_parser()
{
    test_parse_null();
    test_parse_true();
    test_parse_false();
    test_parse_number();
    test_parse_string();
    test_parse_array();

    test_parse_expect_value();
    test_parse_invalid_value();
    test_parse_root_not_singular();
    test_parse_number_too_big();
    test_parse_invalid_string_char();
    test_parse_invalid_string_escape();
    test_parse_miss_quotation_mark();
    test_parse_invalid_unicode_hex();
    test_parse_invalid_unicode_surrogate();
}
int main()
{
    test_parser();
    printf("total %d, pass %d\n", test_count, pass_count);
    return 0;
}