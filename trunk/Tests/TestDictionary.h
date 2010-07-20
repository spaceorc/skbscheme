#include "Test.h"

TEST(SetAndFind) {
	Dictionary dictionary = AllocateDictionary();

	dictionary = InternalSet(dictionary, LimitedStrFromConstantStr(STR("lalala")), Number(1));
	AssertEq(Number(1), InternalFind(dictionary, LimitedStrFromConstantStr(STR("lalala"))));
	AssertThat(0 == InternalFind(dictionary, LimitedStrFromConstantStr(STR("bububu"))));

	dictionary = InternalSet(dictionary, LimitedStrFromConstantStr(STR("bububu")), Number(2));
	AssertEq(Number(1), InternalFind(dictionary, LimitedStrFromConstantStr(STR("lalala"))));
	AssertEq(Number(2), InternalFind(dictionary, LimitedStrFromConstantStr(STR("bububu"))));
	AssertEq(Number(2), InternalFind(dictionary, LimitedStrFromConstantStr(STR("bububu"))));
}