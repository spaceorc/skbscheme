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

TEST(Concatenate) {
	AssertThat(0 == Compare(ConcatenateConstantLimitedStr(LimitConstantStr(STR("lalala")), LimitConstantStr(STR("bububu"))), LimitedStrFromConstantStr(STR("lalalabububu"))));
}

TEST(BuildStrings) {
	StringBuilder * stringBuilder = 0;
	stringBuilder = AppendLimitedStr(stringBuilder, LimitedStrFromConstantStr(STR("lalala1")));
	stringBuilder = AppendConstantStr(stringBuilder, STR("lalala2"));
	stringBuilder = AppendConstantLimitedStr(stringBuilder, ConstLimitedStr(LimitedStrFromConstantStr(STR("lalala3"))));
	stringBuilder = AppendChr(stringBuilder, 'l');
	stringBuilder = AppendChr(stringBuilder, 'a');
	stringBuilder = AppendChr(stringBuilder, 'l');
	stringBuilder = AppendChr(stringBuilder, 'a');
	stringBuilder = AppendChr(stringBuilder, 'l');
	stringBuilder = AppendChr(stringBuilder, 'a');
	stringBuilder = AppendChr(stringBuilder, '4');
	AssertThat(0 == Compare(LimitedStrFromConstantStr(STR("lalala1lalala2lalala3lalala4")), BuildString(stringBuilder)));
}