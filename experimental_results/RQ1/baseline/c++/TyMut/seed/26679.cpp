
struct B {
	struct { int bar; };
};

int main()
{
	B foo;
	(void)foo.bar; // ok
}

The following does not work though:

struct A {
	int bar;
};

struct B {
	struct : A {};
};

int main()
{

	B foo;
	(void)foo.bar;  // error: 'struct B' has no member named 'bar'
}

