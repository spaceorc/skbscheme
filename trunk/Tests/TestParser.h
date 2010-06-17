#include "Test.h"

TEST(ParseEnd) {
	Context * context = 0;
	Token token;
	token.tag = tokEnd;
	AssertThat(0 == Parse(token, &context));
}