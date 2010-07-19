#include "Test.h"

Token BuildToken(int tag, ConstantStr range) {
	Token result;
	result.tag = tag;
	result.text = LimitedStrFromConstantStr(range);
	return result;
}

TEST(ParseNumber) {
	ParserContext * context = AcquireParserContext();
	AssertEq(Number(12345), Parse(BuildToken(tokSymbol, STR("12345")), &context));
}

TEST(ParseConstantString) {
	ParserContext * context = AcquireParserContext();
	AssertEq(ConstantStringFromConstantStr(STR("12345")), Parse(BuildToken(tokQuotedString, STR("12345")), &context));
	AssertEq(ConstantStringFromConstantStr(STR("12345")), Parse(BuildToken(tokQuotedString, STR("12345")), &context));
}

TEST(ParseOpeningBracket) {
	ParserContext * context = AcquireParserContext();
	AssertThat(0 == Parse(BuildToken(tokOpeningBracket, STR("(")), &context));
}

TEST(ParseAndFinish) {
	ParserContext * context = AcquireParserContext();
	AssertThat(0 != CanFinishParsing(context));
	Parse(BuildToken(tokOpeningBracket, STR("(")), &context);
	AssertThat(0 == CanFinishParsing(context));
	Parse(BuildToken(tokClosingBracket, STR(")")), &context);
	AssertThat(0 != CanFinishParsing(context));
}

TEST(ParseWithBrokenBracketBalance) {
	ParserContext * context = AcquireParserContext();
	AssertThat(0 != CanFinishParsing(context));
	Parse(BuildToken(tokOpeningBracket, STR("(")), &context);
	AssertThat(0 == CanFinishParsing(context));
	Parse(BuildToken(tokClosingBracket, STR(")")), &context);
	AssertThat(0 != CanFinishParsing(context));
	AssertTag(terError, Parse(BuildToken(tokClosingBracket, STR(")")), &context));
}

TEST(ParseRedex) {
	ParserContext * context = AcquireParserContext();
	Parse(BuildToken(tokOpeningBracket, STR("(")), &context);
	AssertThat(0 == Parse(BuildToken(tokSymbol, STR("1")), &context));
	Term * was = Parse(BuildToken(tokClosingBracket, STR(")")), &context);
	Term * expected = AllocateTerm(terRedex);
	expected->redex = MakeList(1, Number(1));
	AssertEq(expected, was);
}

TEST(ParseRedexContainingOperatorPlus) {
	ParserContext * context = AcquireParserContext();
	Parse(BuildToken(tokOpeningBracket, STR("(")), &context);
	AssertThat(0 == Parse(BuildToken(tokSymbol, STR("+")), &context));
	AssertThat(0 == Parse(BuildToken(tokSymbol, STR("1")), &context));
	AssertThat(0 == Parse(BuildToken(tokSymbol, STR("2")), &context));
	Term * was = Parse(BuildToken(tokClosingBracket, STR(")")), &context);
	Term * expected = AllocateTerm(terRedex);
	expected->redex = MakeList(3, SymbolFromConstantStr(STR("+")), Number(1), Number(2));
	AssertEq(expected, was);
}

TEST(ParseRedexContainingOperatorMinus) {
	ParserContext * context = AcquireParserContext();
	Parse(BuildToken(tokOpeningBracket, STR("(")), &context);
	AssertThat(0 == Parse(BuildToken(tokSymbol, STR("-")), &context));
	AssertThat(0 == Parse(BuildToken(tokSymbol, STR("1")), &context));
	AssertThat(0 == Parse(BuildToken(tokSymbol, STR("2")), &context));
	Term * was = Parse(BuildToken(tokClosingBracket, STR(")")), &context);
	Term * expected = AllocateTerm(terRedex);
	expected->redex = MakeList(3, SymbolFromConstantStr(STR("-")), Number(1), Number(2));
	AssertEq(expected, was);
}

TEST(ParseRedexContainingNestedRedexes) {
	ParserContext * context = AcquireParserContext();
	Parse(BuildToken(tokOpeningBracket, STR("(")), &context);
	Parse(BuildToken(tokOpeningBracket, STR("(")), &context);
	AssertThat(0 == Parse(BuildToken(tokSymbol, STR("1")), &context));
	AssertThat(0 == Parse(BuildToken(tokClosingBracket, STR(")")), &context));
	Term * was = Parse(BuildToken(tokClosingBracket, STR(")")), &context);
	Term * expected = AllocateTerm(terRedex);
	Term * nested = AllocateTerm(terRedex);
	expected->redex = MakeList(1, nested);
	nested->redex = MakeList(1, Number(1));
	AssertEq(expected, was);
}

TEST(ParseCharacter) {
	ParserContext * context = AcquireParserContext();
	AssertEq(Character('x'), Parse(BuildToken(tokSymbol, STR("#\\x")), &context));
}