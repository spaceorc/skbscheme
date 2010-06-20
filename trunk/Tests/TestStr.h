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