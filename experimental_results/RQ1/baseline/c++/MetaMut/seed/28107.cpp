
struct foo {
	int bar() { return 0; }
	foo* operator->() { return this; }
};

int main()
{
	int pt(foo()->bar());
	return pt;
}
