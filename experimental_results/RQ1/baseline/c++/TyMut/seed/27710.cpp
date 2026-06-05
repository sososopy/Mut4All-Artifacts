

template <class X, bool (X::*fcn)(int param)=0 >
struct test   {
	int v;
};

struct dummy {
	bool fcn(int);
};
using TT = test<dummy, &dummy::fcn>;	// works
using TT = test<dummy, 0>;				// generates ICE
using TT = test<dummy>;					// generates ICE

