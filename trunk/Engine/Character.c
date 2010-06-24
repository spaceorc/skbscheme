#include "Character.h"

// todo unit tests required, especially for incorrect character tokens

Term * ParseCharacter(LimitedStr symbol) {
	return Character(symbol.str[2]);
}

int IsCharacter(LimitedStr symbol) {
	return symbol.size >= 4 && '#' == symbol.str[0] && '\\' == symbol.str[1] && 0 != symbol.str[2] && 0 == symbol.str[3];
}