
template<typename T>
struct A
{
	template<typename... Args>
		constexpr A(Args&&...){}
};

struct B : public A<int>
{
	using A<int>::A;
};

// Uncommenting the following line makes this file compile.
//constexpr B p{0};
// Commenting the following line makes this file compile.
template<typename>
struct X
{
	X()
	{
		// The following line is fine.
		const B b0{0};
		// The following line produces an error.
		constexpr B b1{0};
	}
};
