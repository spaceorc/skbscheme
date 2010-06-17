#pragma once

#include "Test.h"

TEST(GetTokenFromEmptyString) {
	const char * str = "";
	AssertTok(tokEnd, GetToken(&str));
	AssertThat(str == 0);
	AssertTok(tokEnd, GetToken(&str));
}

TEST(GetTokenOpeningBracket) {
	const char * str = "(";
	AssertBracket(tokOpeningBracket, "(", GetToken(&str));
}

TEST(GetTokenClosingBracket) {
	const char * str = ")";
	AssertBracket(tokClosingBracket, ")", GetToken(&str));
}

TEST(GetTokenSymbol) {
	const char * str = "lalala";
	AssertSymbol("lalala", GetToken(&str));
}

TEST(GetTokenSkipsWhitespaces) {
	const char * str = "   bubu";
	AssertSymbol("bubu", GetToken(&str));
}

TEST(GetTokenSkipsWhitespacesBeforeBracket) {
	const char * str = "   (";
	AssertTok(tokOpeningBracket, GetToken(&str));
}

TEST(GetTokenStepByStep) {
	const char * str = "  (  +  a  b  )  ";
	AssertTok(tokOpeningBracket, GetToken(&str));
	AssertSymbol("+", GetToken(&str));
	AssertSymbol("a", GetToken(&str));
	AssertSymbol("b", GetToken(&str));
	AssertTok(tokClosingBracket, GetToken(&str));
	AssertTok(tokEnd, GetToken(&str));
	AssertThat(str == 0);
}