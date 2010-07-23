#include "Test.h"

TEST(IsCharacter) {
	ContextBindings * contextBindings = AcquireContextBindings();
	AssertEq(True(), Eval(ParseSingle("(char? #\\c)"), contextBindings));
	AssertEq(False(), Eval(ParseSingle("(char? #t)"), contextBindings));
}

TEST(CharacterEq) {
	ContextBindings * contextBindings = AcquireContextBindings();
	AssertEq(True(), Eval(ParseSingle("(char?= #\\a #\\a #\\a)"), contextBindings));
	AssertEq(False(), Eval(ParseSingle("(char?= #\\a #\\a #\\b)"), contextBindings));
}

TEST(CharacterEqWith1Argument) {
	ContextBindings * contextBindings = AcquireContextBindings();
	AssertEq(InvalidArgumentCount(), Eval(ParseSingle("(char?= #\\a)"), contextBindings));
}

TEST(CharacterEqWithNotACharacter) {
	ContextBindings * contextBindings = AcquireContextBindings();
	AssertEq(InvalidArgumentType(), Eval(ParseSingle("(char?= 1 #\\a)"), contextBindings));
}