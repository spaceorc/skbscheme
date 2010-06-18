#include "Test.h"

TEST(NumberPlus) {
	Term * z = OperatorPlus(MakeList(2, Number(1), Number(2)));
    AssertThat((z->tag == terNumber) && (z->number == 3));
}

TEST(NumberPlusNoArguments) {
	Term * z = OperatorPlus(MakeList(0));
    AssertThat(z->tag == terError);
}

TEST(NumberPlus3Arguments) {
	Term * z = OperatorPlus(MakeList(3, Number(1), Number(2), Number(3)));
    AssertThat((z->tag == terNumber) && (z->number == 6));
}

TEST(NumberPlus1Argument) {
	Term * z = OperatorPlus(MakeList(1, Number(1)));
    AssertThat(z->tag == terError);
}

TEST(NumberPlus1stIsNotANumber) {
	Term * z = OperatorPlus(MakeList(1, Nil()));
    AssertThat(z->tag == terError);
}

TEST(NumberPlus3Arguments2ndIsNotANumber) {
	Term * z = OperatorPlus(MakeList(3, Number(1), Nil(), Number(3)));
    AssertThat(z->tag == terError);
}

TEST(NumberMinus) {
	Term * z = OperatorMinus(MakeList(2, Number(5), Number(2)));
    AssertThat((z->tag == terNumber) && (z->number == 3));
}

TEST(NumberMinusNoArguments) {
	Term * z = OperatorMinus(MakeList(0));
    AssertThat(z->tag == terError);
}

TEST(NumberMinus1Argument) {
	Term * z = OperatorMinus(MakeList(1, Number(1)));
    AssertThat((z->tag == terNumber) && (z->number == -1));
}

TEST(NumberMinus3Arguments) {
	Term * z = OperatorMinus(MakeList(3, Number(1), Number(2), Number(3)));
    AssertThat((z->tag == terNumber) && (z->number == -4));
}

TEST(NumberMinus1stIsNotANumber) {
	Term * z = OperatorMinus(MakeList(1, Nil()));
    AssertThat(z->tag == terError);
}