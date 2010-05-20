#pragma once

#include "Test.h"
#include "Engine.h"

void TestNumberPlus() {
	Term * z = OperatorPlus(Cons(Number(1), Cons(Number(2), Nil()))->pair);
    AssertThat((z->tag == tagNumber) && (z->number == 3));
}

void TestNumberPlus3Arguments() {
	Term * z = OperatorPlus(Cons(Number(1), Cons(Number(2), Cons(Number(3), Nil())))->pair);
    AssertThat((z->tag == tagNumber) && (z->number == 6));
}

void TestNumberPlus1stIsNotANumber() {
	Term * z = OperatorPlus(Cons(Nil(), Nil())->pair);
    AssertThat(z->tag == tagError);
}

void TestNumberPlusNotAList() {
	Term * z = OperatorPlus(Cons(Number(1), Number(2))->pair);
    AssertThat(z->tag == tagError);
}

void TestNumberPlus3Arguments2ndIsNotANumber() {
	Term * z = OperatorPlus(Cons(Number(1), Cons(Nil(), Cons(Number(3), Nil())))->pair);
    AssertThat(z->tag == tagError);
}

void TestNumberMinus() {
	Term * z = OperatorMinus(Cons(Number(5), Cons(Number(2), Nil()))->pair);
    AssertThat((z->tag == tagNumber) && (z->number == 3));
}

void TestNumber() {
	RunTest(TestNumberPlus);
	RunTest(TestNumberPlus3Arguments);
	RunTest(TestNumberPlus1stIsNotANumber);
	RunTest(TestNumberPlusNotAList);
	RunTest(TestNumberPlus3Arguments2ndIsNotANumber);
	RunTest(TestNumberMinus);
}