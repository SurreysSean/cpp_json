#ifndef TOOLS_HPP
#define TOOLS_HPP
#include <cassert>

#define EXPECT(c, expect_c) (assert(c == expect_c))
bool is_whitespace(const char &);

/* ------------unit test------------------ */
#endif