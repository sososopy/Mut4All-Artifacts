
struct A
{
	int a;
};

struct T
{
	int x;

	T() : x([]{
		sizeof(::A::a);
		return 0;
	}())
	{}
};

