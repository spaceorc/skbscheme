#include "Character.h"

Term * ParseCharacter(LimitedStr symbol) {
	return Character(symbol.str[2]);
}

int IsCharacter(LimitedStr symbol) {
	return symbol.size >= 3 && symbol.str[0] == '#' && symbol.str[1] == '\\' && symbol.str[2] != 0;
}