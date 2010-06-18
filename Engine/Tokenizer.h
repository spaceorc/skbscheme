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
	ConstLimitedStr range;
};

Token GetToken(ConstStr * input);

const char * DumpTok(int tag);