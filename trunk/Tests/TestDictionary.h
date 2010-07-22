#include "Test.h"

TEST(SetAndFind) {
	Dictionary dictionary = CreateDictionary();

	dictionary = Set(dictionary, LimitedStrFromConstantStr(STR("lalala")), Number(1));
	AssertEq(Number(1), Find(dictionary, LimitedStrFromConstantStr(STR("lalala"))));
	AssertThat(0 == Find(dictionary, LimitedStrFromConstantStr(STR("bububu"))));

	dictionary = Set(dictionary, LimitedStrFromConstantStr(STR("bububu")), Number(2));
	AssertEq(Number(1), Find(dictionary, LimitedStrFromConstantStr(STR("lalala"))));
	AssertEq(Number(2), Find(dictionary, LimitedStrFromConstantStr(STR("bububu"))));
	AssertEq(Number(2), Find(dictionary, LimitedStrFromConstantStr(STR("bububu"))));
}