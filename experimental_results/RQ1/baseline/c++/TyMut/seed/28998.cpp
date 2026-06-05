//test.hh
#include <stdio.h>

class Base
{
	int v;
public:
	Base(int u): v(u) {}
	int get() { return v; }
};

class A
{
	int a;
public:
	A(int a_): a(a_) {}
	void print() { printf("%d\n", a); }
};

class B: public Base
{
	A a;
	int b;
public:
	B(int b_);
	void print() { a.print(); }
};
//test.cpp
#include "test.hh"

B::B(int b_):
	Base(b_),
	a(b), b(100)
{}
