
struct C
{
	bool empty();
};

struct D : C
{
	using C::empty;
};

int
main()
{
	if(D().empty); // error expected but got ICE
}

