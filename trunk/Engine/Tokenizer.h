#pragma once

typedef const char * ConstStr;

struct structConstLimitedStr;
typedef struct structConstLimitedStr ConstLimitedStr;

struct structConstLimitedStr {
	ConstStr str;
	unsigned int size;
};

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