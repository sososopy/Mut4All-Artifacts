
template <class Bar, int Bar::*PtrToInt = &Bar::myInt>
class Foo {};

class Bar {
private:
	int myInt;
public:
	void FooBar()
	{
		// does not compile:
		Foo<Bar> test;
		// does compile:
		// Foo<Bar, &Bar::myInt> test;
	}
};
