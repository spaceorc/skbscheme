#include "Test.h"

TEST(ParseEnd) {
	Context * context = AcquireContext();
	Token token;
	token.tag = tokEnd;
	AssertThat(0 == Parse(token, &context));
}

TEST(ParseNumber) {
	Context * context = AcquireContext();
	Token token;
	token.tag = tokSymbol;
	token.range.str = "12345";
	token.range.size = strlen(token.range.str);
	Term * term = AllocateTerm(tagNumber);
	term->number = 12345;
	AssertEq(term, Parse(token, &context));
}

TEST(ParseInvalidNumber) {
	Context * context = AcquireContext();
	Token token;
	token.tag = tokSymbol;
	token.range.str = "12345a";
	token.range.size = strlen(token.range.str);
	AssertTag(tagError, Parse(token, &context));
}

TEST(ParseOpeningBracket) {
	Context * context = AcquireContext();
	Token token;
	token.tag = tokOpeningBracket;
	token.range.str = "(";
	token.range.size = strlen(token.range.str);
	AssertThat(0 == Parse(token, &context));
}