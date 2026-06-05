
template<typename F>
struct Foo {
	
	template<typename T> void foo(void) {}
	
};

template<typename B>
struct Bar {
	
};

int main(void) {
	Foo<double> foo1;
	foo1.foo<int>(); // this line works
	Foo<Bar<double>> foo2;
	foo2.foo<int>(); // this line works
	return 0;
}

template<typename B>
void baz(void) {
	Foo<Bar<B>> foo;
	foo.foo<int>(); // this line gives a compiler error
}
