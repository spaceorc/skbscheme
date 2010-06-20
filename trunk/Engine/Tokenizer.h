#pragma once

#include "Str.h"

#define tokOpeningBracket 0
#define tokClosingBracket 1
#define tokSymbol 2
#define tokEnd 3

struct structToken;
typedef struct structToken Token;

struct structToken {
	int tag;
	LimitedStr range;
};

Token GetToken(LimitedStr * input);

const char * DumpTok(int tag);