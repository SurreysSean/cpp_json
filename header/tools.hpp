#ifndef TOOLS_HPP
#define TOOLS_HPP
#include <cassert>
#include <cctype>
#include <string>
using std::string;

#define EXPECT(c, expect_c) (assert(c == expect_c))
bool is_whitespace(const char &);
bool is_digit1to9(const char &);
string encode_utf8(unsigned);
/* ------------unit test------------------ */
#endif