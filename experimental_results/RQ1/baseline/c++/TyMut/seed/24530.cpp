
template<class>
struct S
{
	static_assert(((S*)0)->~S(), "");
};

S<int> b;

