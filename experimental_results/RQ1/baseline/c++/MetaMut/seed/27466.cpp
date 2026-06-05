
template <class...Ts>
struct tpl
	{
	tpl (Ts...);
	};

template <class...Ts>
struct A 
	: tpl <Ts...>
	{
	template <class...Args>
	A (Args...args)
		: tpl <Ts...> (args...)
		{}
	};

template <class...Ts>
A (Ts...) -> A <Ts...>;

A a {10}; //error: no matching function for call to 'tpl<>::tpl(int&)'
           

