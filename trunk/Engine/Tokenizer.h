#pragma once

#include "Str.h"

#define tokOpeningBracket 0
#define tokClosingBracket 1
#define tokSymbol 2
#define tokEnd 3
#define tokQuotedString 4
#define tokError 5

struct structToken;
typedef struct structToken Token;

struct structToken {
	int tag;
	LimitedStr text;
};

Token GetToken(LimitedStr * input);