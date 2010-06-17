#include "Test.h"

TEST(ParseEnd) {
	Context * context = 0;
	Token token;
	token.tag = tokEnd;
	AssertThat(0 == Parse(token, &context));
	AssertThat(0 == context);
}

TEST(ParseNumber) {
	Context * context = 0;
	Token token;
	token.tag = tokSymbol;
	token.range.str = "12345";
	token.range.size = strlen(token.range.str);
	Term * term = AllocateTerm(tagNumber);
	term->number = 12345;
	AssertEq(term, Parse(token, &context));
	AssertThat(0 == context);
}

TEST(ParseInvalidNumber) {
	Context * context = 0;
	Token token;
	token.tag = tokSymbol;
	token.range.str = "12345a";
	token.range.size = strlen(token.range.str);
	AssertTag(tagError, Parse(token, &context));
	AssertThat(0 == context);
}

TEST(ParseOpeningBracket) {
	Context * context = 0;
	Token token;
	token.tag = tokOpeningBracket;
	token.range.str = "(";
	token.range.size = strlen(token.range.str);
	AssertThat(0 == Parse(token, &context));
	AssertThat(0 != context);
}