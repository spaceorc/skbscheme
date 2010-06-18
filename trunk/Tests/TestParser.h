#include "Test.h"

Token BuildToken(int tag, const char * range) {
	Token result;
	result.tag = tag;
	result.range.str = range;
	result.range.size = strlen(range);
	return result;
}

TEST(ParseNumber) {
	Context * context = AcquireContext();
	Term * term = AllocateTerm(terNumber);
	term->number = 12345;
	AssertEq(term, Parse(BuildToken(tokSymbol, "12345"), &context));
}

TEST(ParseInvalidNumber) {
	Context * context = AcquireContext();
	AssertTag(terError, Parse(BuildToken(tokSymbol, "12345a"), &context));
}

TEST(ParseOpeningBracket) {
	Context * context = AcquireContext();
	AssertThat(0 == Parse(BuildToken(tokOpeningBracket, "("), &context));
}

TEST(ParseAndFinish) {
	Context * context = AcquireContext();
	AssertThat(0 != CanFinishParsing(context));
	Parse(BuildToken(tokOpeningBracket, "("), &context);
	AssertThat(0 == CanFinishParsing(context));
	Parse(BuildToken(tokClosingBracket, ")"), &context);
	AssertThat(0 != CanFinishParsing(context));
}

TEST(ParseWithBrokenBracketBalance) {
	Context * context = AcquireContext();
	AssertThat(0 != CanFinishParsing(context));
	Parse(BuildToken(tokOpeningBracket, "("), &context);
	AssertThat(0 == CanFinishParsing(context));
	Parse(BuildToken(tokClosingBracket, ")"), &context);
	AssertThat(0 != CanFinishParsing(context));
	AssertTag(terError, Parse(BuildToken(tokClosingBracket, ")"), &context));
}

TEST(ParseRedex) {
	Context * context = AcquireContext();
	Parse(BuildToken(tokOpeningBracket, "("), &context);
	AssertThat(0 == Parse(BuildToken(tokSymbol, "1"), &context));
	Term * was = Parse(BuildToken(tokClosingBracket, ")"), &context);
	Term * expected = AllocateTerm(terRedex);
	expected->redex = MakeList(1, Number(1));
	AssertEq(expected, was);
}

TEST(ParseRedexContainingOperatorPlus) {
	Context * context = AcquireContext();
	Parse(BuildToken(tokOpeningBracket, "("), &context);
	AssertThat(0 == Parse(BuildToken(tokSymbol, "+"), &context));
	AssertThat(0 == Parse(BuildToken(tokSymbol, "1"), &context));
	AssertThat(0 == Parse(BuildToken(tokSymbol, "2"), &context));
	Term * was = Parse(BuildToken(tokClosingBracket, ")"), &context);
	Term * expected = AllocateTerm(terRedex);
	expected->redex = MakeList(3, Function(OperatorPlus), Number(1), Number(2));
	AssertEq(expected, was);
}

TEST(ParseRedexContainingOperatorMinus) {
	Context * context = AcquireContext();
	Parse(BuildToken(tokOpeningBracket, "("), &context);
	AssertThat(0 == Parse(BuildToken(tokSymbol, "-"), &context));
	AssertThat(0 == Parse(BuildToken(tokSymbol, "1"), &context));
	AssertThat(0 == Parse(BuildToken(tokSymbol, "2"), &context));
	Term * was = Parse(BuildToken(tokClosingBracket, ")"), &context);
	Term * expected = AllocateTerm(terRedex);
	expected->redex = MakeList(3, Function(OperatorMinus), Number(1), Number(2));
	AssertEq(expected, was);
}

TEST(ParseRedexContainingBoundVariable) {
	Context * context = AcquireContext();
	AddBindingToContext(context, LimitConstStr("a"), Number(5));
	Parse(BuildToken(tokOpeningBracket, "("), &context);
	AssertThat(0 == Parse(BuildToken(tokSymbol, "-"), &context));
	AssertThat(0 == Parse(BuildToken(tokSymbol, "a"), &context));
	AssertThat(0 == Parse(BuildToken(tokSymbol, "2"), &context));
	Term * was = Parse(BuildToken(tokClosingBracket, ")"), &context);
	Term * expected = AllocateTerm(terRedex);
	expected->redex = MakeList(3, Function(OperatorMinus), Number(5), Number(2));
	AssertEq(expected, was);
}