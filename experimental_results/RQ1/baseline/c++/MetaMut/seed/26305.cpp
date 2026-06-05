
struct Foo
{
	void bar() __restrict { }
};

int main()
{
	Foo obj;
	obj.bar();
	return 0;
}

