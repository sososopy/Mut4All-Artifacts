
struct Foo {
	int x;
	void bar() const {
		[=]() { bad(); }(); // This sould be an compiler error
	}
	void bad() {
		x = 42;
	}
};

