struct S
{
	template<typename T>
	operator T() { return T(); }
};

int i = S().template operator int();

