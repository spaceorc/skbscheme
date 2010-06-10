#pragma once

#include "Test.h"

TEST(GetTokenFromEmptyString) {
	const char * str = "";
	AssertTok(tokEnd, GetToken(&str));
	AssertThat(str == 0);
	AssertTok(tokEnd, GetToken(&str));
}