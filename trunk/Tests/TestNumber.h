#pragma once

#include "Test.h"
#include "Engine.h"

TEST(NumberPlus) {
	Term * z = OperatorPlus(Cons(Number(1), Cons(Number(2), Nil()))->pair);
    AssertThat((z->tag == tagNumber) && (z->number == 3));
}

TEST(NumberPlusNoArguments) {
	Term * z = OperatorPlus(0);
    AssertThat(z->tag == tagError);
}

TEST(NumberPlus3Arguments) {
	Term * z = OperatorPlus(Cons(Number(1), Cons(Number(2), Cons(Number(3), Nil())))->pair);
    AssertThat((z->tag == tagNumber) && (z->number == 6));
}

TEST(NumberPlus1Argument) {
	Term * z = OperatorPlus(Cons(Number(1), Nil())->pair);
    AssertThat(z->tag == tagError);
}

TEST(NumberPlus1stIsNotANumber) {
	Term * z = OperatorPlus(Cons(Nil(), Nil())->pair);
    AssertThat(z->tag == tagError);
}

TEST(NumberPlusNotAList) {
	Term * z = OperatorPlus(Cons(Number(1), Number(2))->pair);
    AssertThat(z->tag == tagError);
}

TEST(NumberPlus3Arguments2ndIsNotANumber) {
	Term * z = OperatorPlus(Cons(Number(1), Cons(Nil(), Cons(Number(3), Nil())))->pair);
    AssertThat(z->tag == tagError);
}

TEST(NumberMinus) {
	Term * z = OperatorMinus(Cons(Number(5), Cons(Number(2), Nil()))->pair);
    AssertThat((z->tag == tagNumber) && (z->number == 3));
}

TEST(NumberMinusNoArguments) {
	Term * z = OperatorMinus(0);
    AssertThat(z->tag == tagError);
}