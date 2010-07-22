#include "Test.h"

TEST(CompareEqualStrings) {
	AssertThat(0 == Compare(LimitedStrFromConstantStr(STR("lalala")), LimitedStrFromConstantStr(STR("lalala"))));
	AssertThat(0 == CompareConstantLimitedStr(LimitConstantStr(STR("lalala")), LimitConstantStr(STR("lalala"))));
}

TEST(CompareNotEqualStrings) {
	AssertThat(0 != Compare(LimitedStrFromConstantStr(STR("lalala")), LimitedStrFromConstantStr(STR("bububu"))));
	AssertThat(0 != CompareConstantLimitedStr(LimitConstantStr(STR("lalala")), LimitConstantStr(STR("bububu"))));
	AssertThat(0 != Compare(LimitedStrFromConstantStr(STR("bububu")), LimitedStrFromConstantStr(STR("lalala"))));
	AssertThat(0 != CompareConstantLimitedStr(LimitConstantStr(STR("bububu")), LimitConstantStr(STR("lalala"))));
}

TEST(CompareLess) {
	AssertThat(0 < Compare(LimitedStrFromConstantStr(STR("lalala")), LimitedStrFromConstantStr(STR("bububu"))));
	AssertThat(0 < CompareConstantLimitedStr(LimitConstantStr(STR("lalala")), LimitConstantStr(STR("bububu"))));
	AssertThat(0 > Compare(LimitedStrFromConstantStr(STR("lalala")), LimitedStrFromConstantStr(STR("lalalal"))));
	AssertThat(0 > CompareConstantLimitedStr(LimitConstantStr(STR("lalala")), LimitConstantStr(STR("lalalal"))));
}

TEST(Concatenate) {
	AssertThat(0 == Compare(ConcatenateConstantLimitedStr(LimitConstantStr(STR("lalala")), LimitConstantStr(STR("bububu"))), LimitedStrFromConstantStr(STR("lalalabububu"))));
}

TEST(Length) {
	LimitedStr a = AllocateLimitedStr(6);
	a.str[0] = 'l';
	a.str[1] = 'a';
	a.str[2] = 'l';
	a.str[3] = 0;
	AssertThat(6 == Length(LimitedStrFromConstantStr(STR("lalala"))));
	AssertThat(3 == Length(a));
}