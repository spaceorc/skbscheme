#include "Test.h"

TEST(IsCharacter) {
	ContextBindings * contextBindings = AcquireContextBindings();
	AssertEq(True(), Eval(ParseSingle("(char? #\\c)"), contextBindings));
	AssertEq(False(), Eval(ParseSingle("(char? #t)"), contextBindings));
}


TEST(CharacterEq) {
	AssertEq(True(), FunctionCharacterEq(MakeList(3, Character('a'), Character('a'), Character('a'))));
	AssertEq(False(), FunctionCharacterEq(MakeList(3, Character('a'), Character('a'), Character('b'))));
}

TEST(CharacterEqWith1Argument) {
	AssertEq(InvalidArgumentCount(), FunctionCharacterEq(MakeList(1, Character('a'))));
}

TEST(CharacterEqWithNotACharacter) {
	AssertEq(InvalidArgumentType(), FunctionCharacterEq(MakeList(3, Character('a'), Character('b'), Nil())));
}

TEST(EvalOfOperatorCharacterEq) {
	ContextBindings * contextBindings = AcquireContextBindings();
	AssertEq(False(), Eval(ParseSingle("(char?= #\\c #\\d)"), contextBindings));
}