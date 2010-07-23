#include "Test.h"

TEST(GetTokenFromEmptyString) {
	LimitedStr str = LimitedStrFromConstantStr(STR(""));
	AssertTok(tokEnd, GetToken(&str));
}

TEST(GetTokenOpeningBracket) {
	LimitedStr str = LimitedStrFromConstantStr(STR("("));
	AssertBracket(tokOpeningBracket, STR("("), GetToken(&str));
}

TEST(GetTokenClosingBracket) {
	LimitedStr str = LimitedStrFromConstantStr(STR(")"));
	AssertBracket(tokClosingBracket, STR(")"), GetToken(&str));
}

TEST(GetTokenVariable) {
	LimitedStr str = LimitedStrFromConstantStr(STR("lalala"));
	AssertVariable(STR("lalala"), GetToken(&str));
}

TEST(GetTokenSkipsWhitespaces) {
	LimitedStr str = LimitedStrFromConstantStr(STR("   bubu"));
	AssertVariable(STR("bubu"), GetToken(&str));
}

TEST(GetTokenSkipsWhitespacesBeforeBracket) {
	LimitedStr str = LimitedStrFromConstantStr(STR("   ("));
	AssertTok(tokOpeningBracket, GetToken(&str));
}

TEST(GetTokenStepByStep) {
	LimitedStr str = LimitedStrFromConstantStr(STR("  (  +  a  b  )  "));
	AssertTok(tokOpeningBracket, GetToken(&str));
	AssertVariable(STR("+"), GetToken(&str));
	AssertVariable(STR("a"), GetToken(&str));
	AssertVariable(STR("b"), GetToken(&str));
	AssertTok(tokClosingBracket, GetToken(&str));
	AssertTok(tokEnd, GetToken(&str));
}

TEST(GetTokenBetweenQuotes) {
	LimitedStr str = LimitedStrFromConstantStr(STR("  \"lalala\"  "));
	AssertQuotedString(STR("lalala"), GetToken(&str));
	AssertTok(tokEnd, GetToken(&str));
}

TEST(GetTokenWithUnterminatedQuotedString) {
	LimitedStr str = LimitedStrFromConstantStr(STR("  \"lal"));
	AssertTok(tokError, GetToken(&str));
	AssertTok(tokEnd, GetToken(&str));
}

TEST(GetTokenWithEscapesBetweenQuotes) {
	LimitedStr str = LimitedStrFromConstantStr(STR("  \"lal\\\"ala\" "));
	AssertQuotedString(STR("lal\"ala"), GetToken(&str));
	AssertTok(tokEnd, GetToken(&str));
}