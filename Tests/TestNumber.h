#pragma once

#include "Test.h"

TEST(NumberPlus) {
	Term * z = OperatorPlus(List(2, Number(1), Number(2)));
    AssertThat((z->tag == tagNumber) && (z->number == 3));
}

TEST(NumberPlusNoArguments) {
	Term * z = OperatorPlus(List(0));
    AssertThat(z->tag == tagError);
}

TEST(NumberPlus3Arguments) {
	Term * z = OperatorPlus(List(3, Number(1), Number(2), Number(3)));
    AssertThat((z->tag == tagNumber) && (z->number == 6));
}

TEST(NumberPlus1Argument) {
	Term * z = OperatorPlus(List(1, Number(1)));
    AssertThat(z->tag == tagError);
}

TEST(NumberPlus1stIsNotANumber) {
	Term * z = OperatorPlus(List(1, Nil()));
    AssertThat(z->tag == tagError);
}

TEST(NumberPlus3Arguments2ndIsNotANumber) {
	Term * z = OperatorPlus(List(3, Number(1), Nil(), Number(3)));
    AssertThat(z->tag == tagError);
}

TEST(NumberMinus) {
	Term * z = OperatorMinus(List(2, Number(5), Number(2)));
    AssertThat((z->tag == tagNumber) && (z->number == 3));
}

TEST(NumberMinusNoArguments) {
	Term * z = OperatorMinus(List(0));
    AssertThat(z->tag == tagError);
}

TEST(NumberMinus1Argument) {
	Term * z = OperatorMinus(List(1, Number(1)));
    AssertThat((z->tag == tagNumber) && (z->number == -1));
}

TEST(NumberMinus3Arguments) {
	Term * z = OperatorMinus(List(3, Number(1), Number(2), Number(3)));
    AssertThat((z->tag == tagNumber) && (z->number == -4));
}

TEST(NumberMinus1stIsNotANumber) {
	Term * z = OperatorMinus(List(1, Nil()));
    AssertThat(z->tag == tagError);
}