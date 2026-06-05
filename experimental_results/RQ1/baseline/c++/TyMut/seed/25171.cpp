#include <initializer_list>
#include <iostream>

#define SHOW std::cout << __PRETTY_FUNCTION__ << "\n"

struct Foo {
	Foo() { SHOW; }
	Foo(const Foo &) { SHOW; }
	Foo(Foo &&) { SHOW; }

	Foo(std::initializer_list<Foo>) { SHOW; }
};

int main() {
	Foo b;
	Foo x = Foo { b };
}

