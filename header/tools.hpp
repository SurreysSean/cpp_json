#ifndef TOOLS_HPP
#define TOOLS_HPP
#include <cassert>
#include <cctype>

#define EXPECT(c, expect_c) (assert(c == expect_c))
bool is_whitespace(const char &);
bool is_digit1to9(const char &);
/* ------------unit test------------------ */
#endif