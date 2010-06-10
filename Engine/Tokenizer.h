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
#define tokError 3
#define tokEnd 4

struct structToken;
typedef struct structToken Token;

struct structToken {
	int tag;
	union {
		ConstLimitedStr symbol;
		ConstStr error;
	};
};

Token GetToken(ConstStr * input);

const char * DumpTok(int tag);