#include "Test.h"

Token BuildToken(int tag, const char * range) {
	Token result;
	result.tag = tag;
	result.range.str = range;
	result.range.size = strlen(range);
	return result;
}

TEST(ParseNumber) {
	ParserContext * context = AcquireParserContext();
	Term * term = AllocateTerm(terNumber);
	term->number = 12345;
	AssertEq(term, Parse(BuildToken(tokSymbol, "12345"), &context));
}

TEST(ParseOpeningBracket) {
	ParserContext * context = AcquireParserContext();
	AssertThat(0 == Parse(BuildToken(tokOpeningBracket, "("), &context));
}

TEST(ParseAndFinish) {
	ParserContext * context = AcquireParserContext();
	AssertThat(0 != CanFinishParsing(context));
	Parse(BuildToken(tokOpeningBracket, "("), &context);
	AssertThat(0 == CanFinishParsing(context));
	Parse(BuildToken(tokClosingBracket, ")"), &context);
	AssertThat(0 != CanFinishParsing(context));
}

TEST(ParseWithBrokenBracketBalance) {
	ParserContext * context = AcquireParserContext();
	AssertThat(0 != CanFinishParsing(context));
	Parse(BuildToken(tokOpeningBracket, "("), &context);
	AssertThat(0 == CanFinishParsing(context));
	Parse(BuildToken(tokClosingBracket, ")"), &context);
	AssertThat(0 != CanFinishParsing(context));
	AssertTag(terError, Parse(BuildToken(tokClosingBracket, ")"), &context));
}

TEST(ParseRedex) {
	ParserContext * context = AcquireParserContext();
	Parse(BuildToken(tokOpeningBracket, "("), &context);
	AssertThat(0 == Parse(BuildToken(tokSymbol, "1"), &context));
	Term * was = Parse(BuildToken(tokClosingBracket, ")"), &context);
	Term * expected = AllocateTerm(terRedex);
	expected->redex = MakeList(1, Number(1));
	AssertEq(expected, was);
}

TEST(ParseRedexContainingOperatorPlus) {
	ParserContext * context = AcquireParserContext();
	Parse(BuildToken(tokOpeningBracket, "("), &context);
	AssertThat(0 == Parse(BuildToken(tokSymbol, "+"), &context));
	AssertThat(0 == Parse(BuildToken(tokSymbol, "1"), &context));
	AssertThat(0 == Parse(BuildToken(tokSymbol, "2"), &context));
	Term * was = Parse(BuildToken(tokClosingBracket, ")"), &context);
	Term * expected = AllocateTerm(terRedex);
	expected->redex = MakeList(3, Symbol("+"), Number(1), Number(2));
	AssertEq(expected, was);
}

TEST(ParseRedexContainingOperatorMinus) {
	ParserContext * context = AcquireParserContext();
	Parse(BuildToken(tokOpeningBracket, "("), &context);
	AssertThat(0 == Parse(BuildToken(tokSymbol, "-"), &context));
	AssertThat(0 == Parse(BuildToken(tokSymbol, "1"), &context));
	AssertThat(0 == Parse(BuildToken(tokSymbol, "2"), &context));
	Term * was = Parse(BuildToken(tokClosingBracket, ")"), &context);
	Term * expected = AllocateTerm(terRedex);
	expected->redex = MakeList(3, Symbol("-"), Number(1), Number(2));
	AssertEq(expected, was);
}

TEST(ParseRedexContainingNestedRedexes) {
	ParserContext * context = AcquireParserContext();
	Parse(BuildToken(tokOpeningBracket, "("), &context);
	Parse(BuildToken(tokOpeningBracket, "("), &context);
	AssertThat(0 == Parse(BuildToken(tokSymbol, "1"), &context));
	AssertThat(0 == Parse(BuildToken(tokClosingBracket, ")"), &context));
	Term * was = Parse(BuildToken(tokClosingBracket, ")"), &context);
	Term * expected = AllocateTerm(terRedex);
	Term * nested = AllocateTerm(terRedex);
	expected->redex = MakeList(1, nested);
	nested->redex = MakeList(1, Number(1));
	AssertEq(expected, was);
}