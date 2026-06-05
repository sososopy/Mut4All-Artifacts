
	export module A;

	export void fun(auto&& x) {}

	import A;

	int main()
	{
		fun(42);
	}


